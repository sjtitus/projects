//_______________________________________________________________________________
// Keyboard
// Handle keyboard input
//_______________________________________________________________________________

//_______________________________________________________________________________
// Constructor 
function Keyboard(handler)
{
    console.log("Keyboard: construct");

    // initialize members
    this.direction = "stop";    
    this.handler = handler;
}

Keyboard.prototype.Bind = function()
{
    // HandleKey: capture this as 'me' so that the function can be invoked by
    // an eventhandler
    var me = this; 
    HandleKey = function(e) 
    {
        var c = e.keyCode;
        var t = e.type;
        me.direction = "stop";
        if (t === "keydown")
        { 
            switch (c) {
                case 37: me.direction = "left";       break;
                case 38: me.direction = "up";         break;
                case 39: me.direction = "right";      break;
                case 40: me.direction = "down";       break;
            }
        }
        if (me.handler)
        {
            me.handler(me.direction);
        }
    }
    window.addEventListener("keydown", HandleKey, false);
    window.addEventListener("keyup", HandleKey, false);
}

//_______________________________________________________________________________
// Unit test 
Keyboard.prototype.UnitTest = function()
{
    function handler(direction)
    {
        console.log("Keyboard UnitTest: "+direction);
    }
    this.handler = handler;
    this.Bind(); 
}


