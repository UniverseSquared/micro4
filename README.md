# micro4

micro4 is a work-in-progress fantasy console developed by @UniverseSquared.

## Specs

* Display: 128x128 (scaled 4x)
* Input: 6 Buttons
* Language: Lua

## Usage

At the moment, the only way to get micro4 is to compile it, which can be done on linux as such:

```
$ git clone https://github.com/UniverseSquared/micro4.git
$ cd micro4
$ make
$ # To run: ./micro4 <path to cart>
```

## API

These functions can be used in a micro4 cart.

* `clr(c)` - Clear the screen with the specified color.
* `rect(x,y,w,h,c)` - Draw a rectangle with the specified x, y, width, height and color.
* `rectline(x,y,w,h,c)` - Same as `rect`, except only draws an outline.
* `circle(x,y,radius,c)` - Draw a circle with the specified x, y, radius and color.
* `circleline(x,y,radius,c)` - Same as `circle`, except only draws an outline.
* `btn(n)` - Returns true if the specified button is pressed, false otherwise.
* `btnp(n)` - Returns true if the specified button is pressed and wasn't in the last frame, false otherwise.

The buttons are:

* 0 - Z
* 1 - X
* 2 - Up
* 3 - Down
* 4 - Left
* 5 - Right
