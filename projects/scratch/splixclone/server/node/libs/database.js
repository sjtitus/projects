/*_____________________________________________________________________________
 * database
 * Splixclone database access
 *_____________________________________________________________________________
*/

var mysql           = require('mysql');
var dbconfig        = require('../config/database.js');

console.log('libs/database: creating connection pool (' + dbconfig.connectionLimit +" connections)");

var pool  = mysql.createPool({
  connectionLimit : dbconfig.connectionLimit, 
  host     : dbconfig.host, 
  user     : dbconfig.user, 
  password : dbconfig.password, 
  database : dbconfig.database
});

module.exports = {
    connectionPool: pool
}; 

