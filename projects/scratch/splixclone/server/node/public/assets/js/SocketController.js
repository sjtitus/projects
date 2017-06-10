//_______________________________________________________________________________
// SocketController 
// Handles Web Socket Connectivity 
//_______________________________________________________________________________

//_______________________________________________________________________________
// Constructor 
function SocketController(webSocketURL) 
{
    console.log("SocketController: construct");
    this.url = webSocketURL;
    console.log("SocketController: URL="+this.url);
}

SocketController.prototype.Send = function(msgtype, msgpayload)
{
    var me = this;
    if (me.socket)
    {
        m = '{ "type":"'+msgtype+'", "payload":"'+msgpayload+'"}';
        me.socket.send(m);
    }
}


SocketController.prototype.Connect = function()
{
    var me = this;
    me.socket = new WebSocket(me.url);
/*
    me.socket.addEventListener('open', 
        function(e){
            console.log("SocketController: Open");
        }
    );
    me.socket.addEventListener('close', 
        function(e){
            console.log("SocketController: Close");
        }
    );
    me.socket.addEventListener('error', 
        function(e){
            console.log("SocketController: Error: ");
        }
    );
    me.socket.addEventListener('message', 
        function(msg){
            console.log("SocketController: Message: "+msg.data);
        }
    );
*/
}


//_______________________________________________________________________________
// Unit test 
SocketController.prototype.UnitTest = function()
{
}


