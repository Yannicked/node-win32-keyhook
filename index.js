var ffi = require('ffi');
var ref = require('ref');
var path = require('path')

var keyhook = ffi.Library(path.join(__dirname, 'bin', 'keyhook.'+process.arch+'.dll'), {
		'Create': ['bool', ['pointer', 'pointer']],
		'Destroy': ['bool', []]
});

var cb_down;
var cb_up;

function create() { // first argument is a callback for key_down, second is a argument for key_up. The argument supplied to the callback is a Virtual-Key Code -> https://msdn.microsoft.com/pt-br/library/windows/desktop/dd375731(v=vs.85).aspx
	if (arguments.length == 0) {
		return false;
	}
	cb_down = arguments[0];
	if (arguments.length == 1) {
		cb_up = function() {};
	} else {
		cb_up = arguments[1];
	}	
	c_up = ffi.Callback('void', [ref.types.uint32], cb_up);
	c_down = ffi.Callback('void', [ref.types.uint32], cb_down);
	process.on('exit', function() {
		c_up;
		c_down;
	});
	return keyhook.Create(c_down, c_up);
}

function destroy() { // destroy will clear the callbacks, this should be called before you kill the application
	return keyhook.Destroy();
}


module.exports = {create: create, destroy: destroy};