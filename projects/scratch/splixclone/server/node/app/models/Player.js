/*_____________________________________________________________________________
 * 
 * Player 
 *
 * Splixclone server side player 
 *
 *_____________________________________________________________________________
*/
var net = require('net');

// Constructor
function Player(req)
{
    this.ip = req.ip;
    this.username = req.body.username;
    this.requestTimeStamp = new Date().getTime();
    console.log("Player: constructor: username="+this.username+"|ip="+this.ip+"|reqTime="+this.requestTimeStamp);
}

module.exports = Player; 
