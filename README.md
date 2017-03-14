# 3D Frogger

This code was implemented as part of RMIT University COSC1186/1187 Interactive 3D Graphics and Animation course.

## Controls:

### All modes

| Key | Action      |
| --- | -----       |
| q   | Exit program|
| b   | Pause/Continue|
| m   | Switch between developer mode and game mode|

### Developer mode

| Key         | Action			                 |
| ----        | -----       	   			 |
| Space       | Jump        	   			 |
| v           | Toggle debug mode 			 |
| o           | Toggle axes       			 |
| n           | Toggle normals and the parabola tangent  |
| p           | Toggle wireframe			 |
| l           | Toggle lightining			 |
| w           | Increase speed				 |
| s           | Decrease speed				 |
| a           | Increase angle				 |
| d    	      | Decrease angle				 |
| j    	      | Switch the jumping integration mode	 |
| Left mouse  | Rotate camera				 |
| Left arrow  | Rotate frog left			 |
| Right mouse | Zoom camera				 |
| Right arrow | Rotate frog right			 |
	
	
### Game mode

	| Key              | Action			         |
	| ----             | -----       	   		 |
	| w/Up arrow       | Front jump				 |
	| s/Down arrow     | Back jump				 |
	| a/Left arrow     | Left jump				 |
	| d/Right arrow    | Right jump				 |
	| Mouse right/left | Rotates camera around Y axis	 |

## Build

Simple compilation with gcc compiler:

    $ make
    
## Credits

Some textures and models used are based on creative commons that can be found *[here][LINK1]*, *[here][LINK2]* and *[here][LINK3]*

## Dependencies

This project depends on `OpenGL` libraries to compile.

Therefore, please make sure you have the packages *[OpenGL][OPENGL_LINK]*.

[LINK1]: http://www.theairtightgarage.net/index_textures.html
[LINK2]: http://www.123dapp.com/3dcr-Creature/Racing-frog-8/1625190
[LINK3]: http://www.123dapp.com/123C-3D-Model/Old-Fashioned-Convertible/1124556
[OPENGL_LINK]: https://www.opengl.org/
