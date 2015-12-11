node-win32-keyhook
========
### Node.js Windows keyhook
`node-win32-keyhook` is a library which lets you create a windows hook which will send all the keyboard input to a callback with the [Virtual-Key Code](https://msdn.microsoft.com/pt-br/library/windows/desktop/dd375731(v=vs.85).aspx) as an argument

Example
-------

``` js
var keyhook = require('node-win32-keyhook');

function key_down(keycode) { // A function that converts the keycode to hexadecimal notation
    console.log('Key Down: 0x'+parseInt(keycode).toString(16))
}

function key_up(keycode) { // A function that converts the keycode to hexadecimal notation
    console.log('Key Up: 0x'+parseInt(keycode).toString(16))
}

keyhook.create(key_down, key_up); // Create the hook, and set the key_down and key_up callback

keyhook.destroy(); // Remove the hook



```
Requirements
------------

 * Windows, of course
 * ```DotNet 4.0``` for building the dynamic library

Installation
------------

Make sure you've installed all the necessary build
tools,
then run this command in the source directory:

``` bash
$ npm install node-win32-keyhook
```