## TexturePackerPebble

This repository contains a work-in-progress custom exporter for [TexturePacker](https://www.codeandweb.com/texturepacker), to produce files suitable for a Pebble SDK project.
Feel free to read the explanation on sprite sheets below.

## Support / Issues

Feel free to open an issue here, or to contact [me](http://forums.getpebble.com/profile/77675/DavidStosik) on the [Pebble official forums](forums.getpebble.com)

## How to use

1. Install [TexturePacker](https://www.codeandweb.com/texturepacker) if you haven't already.
2. Create an `exporters/` folder somewhere on your system.

 ```sh
 mkdir -p ~/TexturePacker/exporters
 ```
 
3. Clone this repository inside the folder.

 ```sh
 git clone git@github.com:dstosik/TexturePackerPebble.git ~/TexturePacker/exporters/pebble
 ```
 
4. Start TexturePacker.
5. On the first screen (Open project), you'll have to create a project in an irrelevant framework first, to reach the application's main screen, from which you can go to File>Preferences.
6. On the Preferences' Settings tab, set the "Exporter Directory" field to the `exporters` folder you created above, then click OK.
7. Go back to the "Open project" screen by clicking "New project". You should see a "Pebble" framework now. Select it then click on "Create project".
8. Add your images or smart folders, set up your sprite sheet (for more details, please refer to [TexturePacker's official documentation](https://www.codeandweb.com/texturepacker/documentation)). A few notes:
   - remember that, for an optimal sprite sheet, you want its width to be a multiple of 32, and set its height so that you waste as small area as possible
   - you can set paddings to 0
   - do not try to use advanced features, they probably are not compatible
   - the smart folder feature is awesome: drag and drop a folder to the application, and it will start to watch recursively any file change, to add/update/remove sprites on the fly
   - the names of your sprite image files will be cleaned and used to generate IDs that you can use in your code (example: `digit-0.png` will get the ID `SPRITE_ID_DIGIT_0`)
9. Setup the Pebble framework part, and texture part of your TexturePacker project (the "Data" and "Texture" sections, at the top of the right column):
   - Data Format: Pebble
   - Header file (.h): browse to your Pebble watchapp/watchface project's `src/` folder, then type the name `spritesheet.h` (if you use another name, you'll need to edit files produced by TexturePacker, because of a temporary omission in the software)
   - Code to add to appinfo.json: name the file however you want (say `spritesheet.json`). Once the sprite sheet is published, you'll have to copy its content and paste it into `appinfo.json`.
   - Texture format: PNG
   - Texture file: browse to your Pebble watchapp project's `resources/images` folder then type the name you want for your exported sprite sheet image file
   - Leave the other options as they are, or feel free to play with them as you like
10. Click on the "Publish sprite sheet" button, and let the magic happen!
11. Leave TexturePacker open if you want it to watch your source images folder(s) (you'll still have to click on the "Publish sprite" button manually to update your Pebble app's resources), or close it if you are done.
12. Add the content of the produced JSON file (maybe you named it `spritesheet.json`) to your Pebble app's `appinfo.json`, inside resources:media:. Here's an extract of my `appinfo.json` as example:
  
 ```js
  "companyName": "David Stosik",
  "longName": "Pebblot",
  "projectType": "native",
  "resources": {
    "media": [
      {
        "menuIcon": true,
        "file": "images/appicon.png",
        "name": "IMAGE_MENU_ICON",
        "type": "png"
      }, // <= Don't forget to add a comma here!
      {  // This part, starting here ...
        "file": "images/spritesheet0.png",
        "name": "SPRITESHEET0",  // Name will depend on your choices.
        "type": "png"
      } // ... and ending here, was what I had to copy.
    ]
  },
  "shortName": "Pebblot",
  ```

13. Copy or link the source and header files `texture_packer.*` to your Pebble app's `src/` folder.

  ```
  cp ~/TexturePacker/exporters/pebble/texture_packer.* ~/pebble/MyWatchApp/src/
  #OR
  ln -s ~/TexturePacker/exporters/pebble/texture_packer.* ~/pebble/MyWatchApp/src/
  ```
  The benefit of using a symbolic link is that if you update the TP texture exporter, then those two files will automatically be updated. The inconvenient is that if you use Git or any source control software, and share your project, these two files will appear as simple symbolic links, and the project won't be usable as is by someone else.
14. You can start using your sprite sheet in your code!

  ```c
  #include "spritesheet.h"
  
  // (...)
  
    GBitmap* digit0_sprite = gbitmap_create_with_sprite(SPRITE_ID_DIGIT_0);
  ```
  This will automatically load the sprite sheet in memory if necessary, then keep it in the heap, until you call `texture_destroy_all()` or `texture_destroy(TEXTURE_ID_DIGITS);`. It means that next calls to `gbitmap_create_with_sprite()`, even for another sprite, won't require to load the sprite sheet resource, it's already available.

## Using a sprite sheet on Pebble SDK

This tutorial/real story will probably sound dumb to people who are used to coding on Pebble, or other similar environments, but I'm sure it can help other people (and the bonus at the end might please much many people!).
I figured this out after talking with [@pedrolane](http://forums.getpebble.com/profile/6493/pedrolane), who hinted that if my images are far from 32px wide, then a sprite sheet could be a good idea. Thanks for tutoring me!

So I've been working on a watch face recently ([another Rorschach](http://forums.getpebble.com/discussion/20250/watchface-wip-my-own-attempt-at-yet-another-inkblot-watchface)), and it turns out I need a lot of image resources (each digit being different, I'll need 29 digits display any time in 24-hour format, plus many other small bitmaps).
As it's my first work on Pebble, I went the straightest way: have each image declared as a resource in appinfo.json, then write helper functions that will load and unload the bitmaps from the resources.
At some point, thinking it might be better not to load resources too often (apparently, it uses CPU, thus battery), I tried to cache each of them in a static variable, to retrieve it next time I need to display it.
Turns out it uses a lot of memory in the heap!
A thing to know about bitmaps in Pebble's SDK, is that their rows must be a multiple of 32 pixels (4 bytes) long.
If not, then useless padding is added to get the required size. This padding is irrelevant data for your application, but it takes up memory!
Let's do some maths.

A [`GBitmap`](http://developer.getpebble.com/docs/c/group___graphics_types.html#struct_g_bitmap) is:

 - some private attributes I don't know the size of (but it's probably always the same, and hopefully not enough to care about, so I'll ignore it, basically because I don't know)
 - a `GRect`, that's a `GPoint` and a `GSize`, which are each a couple of 2-Byte scalars (hence, 8 Bytes)
 - a pointer address (1 Byte, am I right? or is it 2?)
 - the number of bytes per row, 2 more bytes

Pretty static, not really interesting. BUT, of course the `GBitmap` also stores the pixels data somewhere. That would be `H * nb` Bytes (where H is the number of lines, and nb the number of Bytes per row).

The number of bytes per row is basically the number of "bits per row" (ie. columns), ceiled to the closest multiple of 32, the whole thing then divided by 8 (because 1 Byte = 8 bits).
We get our formula for the memory used by the actual image data: `H * ceil(W/32)*4` Bytes

So a `GBitmap` weighs `11 + H * ceil(W/32)*4` Bytes in memory.


Now I'm gonna take the example of the watch face I'm working on at the moment. I need two types of digits:

  - 16 small ones, 25x55 pixels (`11 + 55*4 = 231B`)
  - 13 big ones, 27x59 pixels (`11 + 59*4 = 247B`)

![Zero](https://raw.githubusercontent.com/dstosik/TexturePackerPebble/master/tuto/zero.png)

*The pink zone is the memory wasted for 32-bit padding.*

If I want to keep them all in the heap, at the same time, I'm going to use `16*231 + 13*247 = `**6907** Bytes of memory!

Now let's say I take all my resources, pack them together in one big `GBitmap`. Well, first of all the 11+ Bytes that each `GBitmap` needs will be required only once. And I'm also going to gain on the useless 32-bit padding.
There are two apparently efficient ways to pack all my digits in sprite sheet(s):

  - First one would be to line them all horizontally. The big image will be 59px tall, so I'm gonna waste 4 pixels for every column of a small digit.
  - Second one, to fix that, would be to make two sprite sheets, 55 and 59px high. But I'd lose more on 32-bit padding.

In my case (and that's NOT an absolute truth), the second one has a very slighter gain. Maths (again) for the first one:

![One line](https://raw.githubusercontent.com/dstosik/TexturePackerPebble/master/tuto/oneline.png)

I need `16*25 + 13*27 = 751` columns. `(16*25 + 13*27)/32 = 23.46`. Too bad, looks like I'm still going to spend a lot on 32-bit padding (the closest you are to the next whole number, the better). So 24 packs of 4 Bytes per row, on 59 rows. That's 5664 Bytes to which I need to add the 11+ of a `GBitmap` structure. **5675** Bytes total.

![Not bad.](http://29.media.tumblr.com/tumblr_lltzgnHi5F1qzib3wo1_400.jpg)

And maths for the second one: `11 + 55*ceil(16*25/32)*4 + 11 + 59*ceil(13*27/32)*4 = `**5478** Bytes. More than a 20% gain, awesome!

![Two lines](https://raw.githubusercontent.com/dstosik/TexturePackerPebble/master/tuto/twolines.png)

![Fuck yeah.](http://i3.kym-cdn.com/photos/images/newsfeed/000/120/220/85f.jpg)

You'll have to store in your code, somehow, the position of each sprite in the sheet (origin + size: it's a GRect). Either use `#define` macros, which will be replaced during compilation, ~~and use no memory~~ (like this:
```c
#define SPRITE_POS_DIGIT0_0 GRect(0,0,27,59)
```
), or constants, but then I don't know if they use memory (8 bytes per each), or not. (Erratum: [hmm, not exactly how it works...](http://www.reddit.com/r/pebbledevelopers/comments/2uds2z/question_pebble_memory_is_it_cheaper_to_use/cob2t0k) Obviously, whichever way you choose to store your sprite positions, it's gonna take up some memory somewhere, either the app's code size, or the heap, which has the same result, as they share 24kB.)

Once you get to that point, you'll only need to use the [`gbitmap_create_as_sub_bitmap()`](http://developer.getpebble.com/docs/c/group___graphics_types.html#ga5d86515990747e47a76c0a16ed6b2850) function to pick sprites on your sheet:

```c
GBitmap* my_sprite = gbitmap_create_as_sub_bitmap(spritesheet, SPRITE_POS_DIGIT0_0);
```

"No deep-copying occurs", which means you're just going to use 11+ more bytes each time you need a sprite.

Pretty cool, right?

Now making, and maintaining, a sprite sheet is tedious, especially when you're starting to add a lot of small sprites, it gets hard to pack them the best may, and to measure all dimensions and positions.

I discovered [TexturePacker](https://www.codeandweb.com/texturepacker) tonight, played with it a while, then I started to write a custom exporter that will export a PNG sprite sheet file, and a couple .c/.h files for Pebble SDK. Just include the .h file in your main.c (or any source file from which you want to use the sprite sheet), and call `gbitmap_create_with_sprite(SPRITE_ID_YOUR_ID);` to get a `GBitmap` sprite. You'll find the ids in the .h file, they are generated based on your original images filenames.
Don't forget to call `destroy_sprite();` when you leave the application, or if you don't need the sprite sheet (nor any sprite you got from it) and want to free some memory.


## Thanks

Thanks to the people at CodeAndWeb for the technical support and the free license!
