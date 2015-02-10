## TexturePackerPebble

This repository contains a work-in-progress custom exporter for [TexturePacker](https://www.codeandweb.com/texturepacker), to produce files suitable for a Pebble SDK project.
Feel free to read the explanation on sprite sheets below.

## Using a sprite sheet on Pebble SDK

This tutorial/real story will probably sound dumb to people who are used to coding on Pebble, or other similar environments, but I'm sure it can help other people (and the bonus at the end might please much more people!).
I figured this out after talking with [@pedrolane](http://forums.getpebble.com/profile/6493/pedrolane), who hinted that if my images are far from 32px wide, then a sprite sheet could be a good idea. Thanks for tutoring me!

So I've been working on a watch face recently ([another Rorschach](http://forums.getpebble.com/discussion/20250/watchface-wip-my-own-attempt-at-yet-another-inkblot-watchface)), and it turns out I need a lot of image resources (each digit being different, to support 24-hour display, that's already 29 digits, plus many others).
As it's my first work on Pebble, I went the straightest way: have each image declared as a resource in appinfo.json, then write helper functions that will load and unload the bitmaps from the resources.
At some point, thinking it might be better not to load resources too often (apparently, it uses CPU, thus battery), I tried to cache each of them in a static variable, to retrieve it next time I need to display it.
Turns out it uses a lot of memory in the heap!
A thing to know about bitmaps in Pebble's SDK, is that their rows must be a multiple of 32 pixels (4 bytes) long.
If not then useless padding is added to get the required size. This padding is irrelevant data for your application, but it takes up memory!
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

If I want to keep them all in the heap, at the same time, I'm going to use `16*231 + 13*247 = 6907` Bytes of memory!

Now let's say I take all my resources, pack them together in one big `GBitmap`. Well first of all the 11+ Bytes that each `GBitmap` needs will be required only once. And I'm also going to gain on the useless 32-bit padding.
There are two apparently efficient ways to pack all my digits in sprite sheet(s):

  - First one would be to line them all horizontally. The big image will be 57px tall, so I'm gonna waste 4 pixels for every column of a small digit.
  - Second one, to fix that, would be to make two sprite sheets, 55 and 57px high. But I'd lose more on 32-bit padding.

In my case (and that's NOT an absolute truth), the second one has a very slighter gain. Maths (again) for the first one:

![One line](https://raw.githubusercontent.com/dstosik/TexturePackerPebble/master/tuto/oneline.png)

I need `16*25 + 13*27 = 751` columns. `(16*25 + 13*27)/32 = 23.46`. Too bad, looks like I'm still going to spend a lot on 32-bit padding (the closest you are to the next whole number, the better). So 24 packs of 4 bytes per row, on 59 rows. That's 5664px to which I need to add the 11+ of a `GBitmap` structure. 5675 Bytes total.

![Not bad.](http://29.media.tumblr.com/tumblr_lltzgnHi5F1qzib3wo1_400.jpg)

And maths for the second one: `11 + 55*ceil(16*25/32)*4 + 11 + 59*ceil(13*27/32)*4 = 5478` Bytes. More than a 20% gain, awesome!

![Two lines](https://raw.githubusercontent.com/dstosik/TexturePackerPebble/master/tuto/twolines.png)

![Fuck yeah.](http://i3.kym-cdn.com/photos/images/newsfeed/000/120/220/85f.jpg)

You'll have to store in your code, somehow, the position of each sprite in the sheet (origin + size: it's a GRect). Either use `#define` macros, which will be replaced during compilation, and use no memory([erratum: hmm, not exactly how it works...](http://www.reddit.com/r/pebbledevelopers/comments/2uds2z/question_pebble_memory_is_it_cheaper_to_use/cob2t0k)) (like this: `#define SPRITE_POS_DIGIT0_0 GRect(0,0,27,59)` ), or constants, but then I don't know if they use memory (8 bytes per each), or not.

Once you get to that point, you'll only need to use the function [`gbitmap_create_as_sub_bitmap()`](http://developer.getpebble.com/docs/c/group___graphics_types.html#ga5d86515990747e47a76c0a16ed6b2850) to pick sprites on your sheet. `GBitmap* my_sprite = gbitmap_create_as_sub_bitmap(spritesheet, SPRITE_POS_DIGIT0_0);`
"No deep-copying occurs", which means you're just going to use 11+ more bytes each time you need a sprite.

Pretty cool, right?

Now making, and maintaining, a sprite sheet is tedious, especially when you're starting to add a lot of small sprites, it gets hard to pack them the best may, and to measure all dimensions and positions.

I discovered [TexturePacker](https://www.codeandweb.com/texturepacker) tonight, played with it a bit, then I started to write a custom exporter that will export a sprite sheet PNG file, and a couple .c/.h files for Pebble SDK. Just include the .h file in your main.c (or any source file from which you want to use the sprite sheet), and call `gbitmap_create_with_sprite(SPRITE_ID_YOUR_ID);` to get a `GBitmap` sprite. You'll find the ids in the .h file, they are generated based on your original images filenames.
Don't forget to call `destroy_sprite();` when you leave the application, or if you don't need the spritesheet (nor any sprite you got from it) and want to free some memory.

The exporter is a work-in-progress, still needs some refinement (`MultiPackFiles` does not work right now), but I was able to produce files that I can use in my watch face code, so that's that.

Feel free to have a look! [TexturePackerPebble](https://github.com/dstosik/TexturePackerPebble) on my Github account.
