## TexturePackerPebble

This repository contains a work-in-progress custom exporter for [TexturePacker](https://www.codeandweb.com/texturepacker), to produce files suitable for a Pebble SDK project.
Feel free to read the explanation on sprite sheets below.

## Using a sprite sheet on Pebble SDK

I moved this story to my blog: [Using a sprite sheet on Pebble](http://dstosik.github.io/pebble/2015/02/11/using_sprite_sheet_pebble_sdk/). Feel free to take a look if you want to know how I came up writing this quick project, and what kind of gains you can expect from making a sprite sheet for your Pebble application.

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
  This will automatically load the sprite sheet in memory if necessary, then keep it in the heap, until you call `texture_destroy_all()` or `texture_destroy(TEXTURE_ID_DIGITS);` (note that you can find the sprite IDs in `spritesheet.h`). It means that next calls to `gbitmap_create_with_sprite()`, even for another sprite, won't require to load the sprite sheet resource, it's already available.

15. Call `texture_destroy_all();` or `texture_destroy(TEXTURE_ID_TEXTURE_NAME);` when you're done using your sprite sheet or any sprite you got from it. (You should call it at least on the app's deinit).

## Thanks

Thanks to the people at CodeAndWeb for the technical support and the free license!
