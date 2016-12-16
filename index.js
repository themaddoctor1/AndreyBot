//Command line arguments
var args = process.argv.slice(2);
console.log("Running program with token " + args[0]);

//Import discord.js
var Discord = require('discord.js');

//Get the spawn command
var spawn = require('child_process').spawnSync;

//Create an instance of the client
var client = new Discord.Client();

//The bot's token
var token = args[0];

//Run when the bot is ready to run
client.on('ready', () => {
    console.log('Ready to process messages.');
});

//Global variables that control the bot's message rate
var counter;
var resetMessageTimer = () => { counter = 7 * Math.random(); };

//Reset the message timer before anything
resetMessageTimer();

//Runs the event whenever a message is received
client.on('message', message => {
    

    console.log(message.author.username + ": '" + message.content + "'");
    
    var params = 'chain words.txt links.txt ' + message.content;
    var args = params.split(" ");

    //Run the markov program if the string has enough words
    if (args.length >= 2) {
        var proc = spawn('./chainfactory.out', args);
    
    }
    
    //Occasionally posts a message
    if (counter <= 0) {
        args = ['gen', 'words.txt', 'links.txt', 5 + 11 * Math.random()];

        var proc = spawn('./chainfactory.out', args);

        var mssg = proc.stdout.toString();
        message.channel.sendMessage(mssg);

        resetMessageTimer();

    } else {
        console.log("Wait " + counter);
    }

    counter--;

});

//Log in
client.login(token);

