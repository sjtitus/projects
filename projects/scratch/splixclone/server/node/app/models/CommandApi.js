/*_____________________________________________________________________________
 * 
 * CommandApi
 *
 * API for sending commands to the splixclone game server and receiving 
 * responses.
 *
 * Commands:
 *
 *     CreatePlayer: create a new player
 * 
 *_____________________________________________________________________________
*/
var net = require('net');

// Singleton instance
var instance = null;


// Constructor
function CommandApi()
{
    var me = this;

    // Persistent client socket for game server control communications
    this.command_path = '/tmp/splixcommand';
    this.command_socket = null;

    // Command socket event handlers
    this._socketErrorHandler = function(err)
    {
        console.log('CommandApi: error on command socket: '+error.message); 
    }

    this._socketDataHandler = function(data)
    {
        console.log('CommandApi: data on command socket: '+data);
        me.HandleResponse(data); 
    }

    this._socketCloseHandler = function(had_error)
    {
        console.log('CommandApi: command socket closed, had_error='+had_error); 
    }

    this._socketConnectHandler = function()
    {
        console.log('CommandApi: command socket connected'); 
    }

    
}


//_____________________________________________________________________________
// Initialize
//
CommandApi.prototype.Initialize = function() 
{
    var me = this;

    if (this.command_socket)
    {
        throw "CommandApi::Initialize: command socket already created!";
    }
    
    console.log('CommandApi::Initialize: creating command socket');
    this.command_socket = new net.Socket();
    
    console.log('CommandApi::Initialize: setting command socket encoding to UTF8'); 
    this.command_socket.setEncoding('utf8');
    
    console.log('CommandApi::Initialize: installing handlers'); 
    this.command_socket.on('error', this._socketErrorHandler); 
    this.command_socket.on('data', this._socketDataHandler); 
    this.command_socket.on('close', this._socketCloseHandler); 
    this.command_socket.on('connect', this._socketConnectHandler); 
   
    // connect to the game server  
    console.log('CommandApi::Initialize: connecting to game server ('+this.command_path+')'); 
    this.command_socket.connect( this.command_path );
}


CommandApi.prototype.HandleResponse = function(data)
{
}


//_____________________________________________________________________________
// CreatePlayer
// Create a player
CommandApi.prototype.CreatePlayer = function(newPlayer)
{
    console.log("CommandApi::CreatePlayer: username="+newPlayer.username+"|ip="+newPlayer.ip+"|reqTime="+newPlayer.requestTimeStamp);
    this.command_socket.write("create player "+JSON.stringify(newPlayer)+"\n");
}


function Instance()
{
    if (!instance)
    {
        console.log('CommandApi: creating/initializing CommandApi singleton');
        instance = new CommandApi;
        instance.Initialize(); 
    }
    else
    {
        console.log('CommandApi: returning already existing CommandApi'); 
    } 
    return instance;
}


module.exports = { Instance: Instance } 

