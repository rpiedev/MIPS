const realRoute = {
    host: {
        guid: "7C",
        type: 0,
        sub: [
            {
                guid: "A2",
                type: 1,
                sub: [
                    {
                        guid: "G9",
                        type: 1,
                        sub: []
                    },
                    {
                        guid: "KB",
                        type: 1,
                        sub: []
                    }
                ]
            }
        ]
    }
};

const modDict = {
    0: { name: "Host", out: 1, size_x: 30, size_y: 30, color: "#343434" },
    1: { name: "example", out: 2, size_x: 30, size_y: 30, color: "#9d9d9d" }
}

/*
Enum for Packet.type
    0 : undefined
    1 : HOST
    2 : MOD
*/
class Packet {
    type = 0;
    route = [];
    constructor(type) {
        this.type = type
    }
}

class Module {
    guid = "";
    modId = 0;
    modName = "";
    inputMod = undefined;
    outputMods = [];
    constructor(guid, modId) {
        this.guid = guid;
        this.modId = modId;
        this.modName = modDict[modId].name;
    }
    Add(mod) {
        if (modDict[this.modId].out >= this.outputMods.length) {
            this.outputMods.push(mod);
            mod.inputMod = this;
        } 
    }
    HandlePacket(name, dir, dataString) {
        // pretend this is like an actual packet being sent and the handler finding it naturally and not just
        // me calling the handler directly this wouldnt really happen
        if (name == "HOST") {
            dataString += this.guid;
        }
        switch (dir) {
            case 0: // send packet to input
                if (!this.input) {
                    console.log(dataString);
                    break;
                }
                this.inputMod.HandlePacket(name, dir, dataString);
                break;    
            case 1: // send packet to all outputs
                if (this.outputMods.length == 0) {
                    this.HandlePacket("MOD", 0, dataString);
                    break;
                }
                this.outputMods.forEach(outputMod => {
                    outputMod.HandlePacket(name, dir, dataString);
                })
                break;
        }
    }
}

//* ================================================

var calculatedRoute = { host: {} };

function init() {
    canvas = document.getElementById("canvas");
    ctx = canvas.getContext("2d");

    BB = canvas.getBoundingClientRect();
    offsetX = BB.left;
    offsetY = BB.top;   

    canvas.onmousedown = mdown;
    canvas.onmouseup = mup;
    canvas.onmousemove = mmove;

    var host = navTree(realRoute.host);
    
    console.log(host)

    host.HandlePacket("HOST", 1, "");

    //draw();
}
function navTree(mod) {
    let thisMod = new Module(mod.guid, mod.type);
    mod.sub.forEach(subMod => {
        thisMod.Add(navTree(subMod));
    });
    return thisMod;
}

// * ================================ CANVAS STUFF

var canvas;
var ctx;
var BB;
var offsetX;
var offsetY;
var things = [];
var WIDTH = 1500;
var HEIGHT = 500;

var dragok = false;
var startX;
var startY;

function rect(x, y, w, h) {
    ctx.beginPath();
    ctx.rect(x, y, w, h);
    ctx.closePath();
    ctx.fill();
}
function draw() {
    clear();
    things.forEach(thing => {
        ctx.fillStyle = thing.fill;
        rect(thing.x, thing.y, thing.width, thing.height);
    });
}
function clear() {
    ctx.clearRect(0, 0, WIDTH, HEIGHT);
    ctx.fillStyle = "#FAF7F8";
    rect(0, 0, WIDTH, HEIGHT);
}
   
function mdown(e) {
    
    e.preventDefault();
    e.stopPropagation();

    var mx = parseInt(e.clientX - offsetX);
    var my = parseInt(e.clientY - offsetY);

    dragok = false;
    for (let i = 0; i < things.length; i++) {
        let thing = things[i];
        if (mx > thing.x && mx < thing.x + thing.width && my > thing.y && my < thing.y + thing.height) {
            dragok = true;
            thing.isDragging = true;
            break;
        }
    }

    startX = mx;
    startY = my;
}
   
function mup(e){
    
    e.preventDefault();
    e.stopPropagation();

    dragok = false;
    things.forEach(thing => {
        thing.isDragging = false;
    });
}

function mmove(e) {
    if (!dragok) return;

    e.preventDefault();
    e.stopPropagation();

    var mx = parseInt(e.clientX - offsetX);
    var my = parseInt(e.clientY - offsetY);

    var dx = mx - startX;
    var dy = my - startY;

    things.forEach(thing => {
        if (thing.isDragging) {
            thing.x += dx;
            thing.y += dy;
        }
    });

    draw();

    startX = mx;
    startY = my;
}