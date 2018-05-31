// const puppeteer = require('puppeteer');
// const request = require('request');

const fs = require('fs');
const url = require('url');
const path = require('path');
const util = require('util');
const http = require('http');
const execSync = require('child_process').execSync;

var log_id = logid();
function log(s){
	var p = '/home/chronos/user/tv/logs/log_'+log_id+'.txt';
	var d = (new Date()).toISOString();
	console.log('write', p, d);
	if(!fs.existsSync(p)){
	}
	fs.appendFileSync(p, d+": "+s+"\n");
}

/*
request("http://localhost:9222/json/list", {json:true}, function(err, resp, body){
	log("Found: "+JSON.stringify(body));
});
*/

if(process.argv[2]!='dev'){
	execSync("iptables -I INPUT -p tcp --dport 3130 -j ACCEPT");
}


http.createServer(function(req,res){
	res.end("OK");
}).listen(3130);


log('running...');

process.on("SIGTERM", function(){
	log('exiting');
	process.exit(0);
});

function logid(){
	var d = new Date();
	return [
		d.getFullYear(),
		d.getMonth(),
		d.getDate(),
		d.getHours(),
		d.getMinutes(),
		d.getSeconds(),
		d.getMilliseconds()
	].join('_');
}