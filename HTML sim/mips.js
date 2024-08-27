const letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const modDict = {
    0: { name: "Host", out: 1, size_x: 30, size_y: 30, color: "#fcba03" },
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
    HandlePacket(name, dataString) {
        // pretend this is like an actual packet being sent and the handler finding it naturally and not just
        // me calling the handler directly this wouldnt really happen
        switch (name) {
            case "MOD": // send packet to input
                if (!this.inputMod) {
                    console.log(dataString);
                    return;
                }
                this.inputMod.HandlePacket(name, dataString);
                break;
            case "HOST": // send packet to all outputs
                dataString += ":"+this.guid;
                if (this.outputMods.length == 0) {
                    this.HandlePacket("MOD", dataString);
                    break;
                }
                this.outputMods.forEach(outputMod => {
                    outputMod.HandlePacket(name, dataString);
                })
                break;
        }
    }
}

//* ================================================

var calculatedRoute = { host: {} };
var host;

function init() {
    canvas = document.getElementById("canvas");
    ctx = canvas.getContext("2d");

    BB = canvas.getBoundingClientRect();
    offsetX = BB.left;
    offsetY = BB.top;   

    canvas.onmousedown = mdown;
    canvas.onmouseup = mup;
    canvas.onmousemove = mmove;
    canvas.oncontextmenu = (e) => e.preventDefault();

    host = createModule("C7", 0);

    genGuid();

    draw();
}
function calcRoute() {
    console.log(host);
    host.HandlePacket("HOST", "");
}
function navTree(mod) {
    let thisMod = new Module(mod.guid, mod.type);
    mod.sub.forEach(subMod => {
        thisMod.Add(navTree(subMod));
    });
    return thisMod;
}
function genGuid() {
    let randLetter = letters[Math.floor(Math.random() * 26)];
    let randNumber = Math.floor(Math.random() * 10);
    document.getElementById("guid").value = randLetter + randNumber;
}

// * ================================ CANVAS STUFF

var canvas;
var ctx;
var BB;
var offsetX;
var offsetY;
var things = [];
var connections = [];
var memory = [];
var WIDTH = 1500;
var HEIGHT = 500;

var dragok = false;
var startX;
var startY;

function createModule(guid = document.getElementById("guid").value, modId = document.getElementById("type").value) {
    if (memory.includes(guid)) {
        console.error("Module with GUID " + guid + " already exists!");
        return;
    }
    let randx = 100+Math.floor(Math.random() * 100 - 50);
    let randy = 100 + Math.floor(Math.random() * 100 - 50);
    let dict = modDict[modId];
    var newMod = new Module(guid, modId);
    memory.push(guid);
    things.push({ x: randx, y: randy, fill: dict.color, width: dict.size_x, height: dict.size_y, isDragging: false, mod: newMod });
    draw();
    genGuid();
    return newMod;
}

function rect(x, y, w, h) {
    ctx.beginPath();
    ctx.rect(x, y, w, h);
    ctx.closePath();
    ctx.fill();
}
function line(x1, y1, x2, y2) {
    ctx.beginPath();
    ctx.moveTo(x1, y1);
    ctx.lineTo(x2, y2);
    ctx.stroke();
}
function draw() {
    clear();
    things.forEach(thing => {
        ctx.fillStyle = thing.fill;
        rect(thing.x, thing.y, thing.width, thing.height);
        ctx.font = "20px serif";
        ctx.fillStyle = "#000";
        ctx.fillText(thing.mod.guid, thing.x, thing.y+thing.height-8);
    });
    connections.forEach(conn => {
        ctx.fillStyle = "#000000";
        if (conn.isDragging) {
            line(conn.start.x+conn.start.width/2, conn.start.y+conn.start.height/2, conn.end.x, conn.end.y);
        } else {
            line(conn.start.x+conn.start.width/2, conn.start.y+conn.start.height/2, conn.end.x+conn.start.width/2, conn.end.y+conn.start.height/2);
        }
        
    })
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

    if (e.buttons == 1) {
        dragok = false;
        for (let i = 0; i < things.length; i++) {
            let thing = things[i];
            if (mx > thing.x && mx < thing.x + thing.width && my > thing.y && my < thing.y + thing.height) {
                if (e.altKey) { 
                    if (thing.mod.modName == "Host") {
                        console.error("Cannot remove Host");
                        return;
                    }
                    things.splice(things.indexOf(thing), 1);
                    return;
                } 
                dragok = true;
                thing.isDragging = true;
                break;
            }
        }

        startX = mx;
        startY = my;
    } else if (e.buttons == 2) {
        dragok = false;
        for (let i = 0; i < things.length; i++) {
            let thing = things[i];
            if (mx > thing.x && mx < thing.x + thing.width && my > thing.y && my < thing.y + thing.height) {
                if (thing.mod.outputMods.length >= modDict[thing.mod.modId].out) {
                    console.error("Module " + thing.mod.guid + " has reached maximum outputs");
                    return;
                }
                dragok = true;
                connections.push({ start: thing, end: {x:thing.x,y:thing.y}, isDragging: true });
                break;
            }
        }

        startX = mx;
        startY = my;
    }
}
   
function mup(e){
    
    e.preventDefault();
    e.stopPropagation();

    var mx = parseInt(e.clientX - offsetX);
    var my = parseInt(e.clientY - offsetY);

    var has = true;
    dragok = false;
    things.forEach(thing => {
        thing.isDragging = false;
        if (mx > thing.x && mx < thing.x + thing.width && my > thing.y && my < thing.y + thing.height) {
            has = false;
            connections.forEach(conn => {
                if (conn.isDragging) {
                    if (thing.mod.modName == "Host") {
                        console.error("Cannot connect to input of Host");
                        has = true;
                        return;
                    } else if (conn.start.mod.guid == thing.mod.guid) {
                        console.error("Cannot connect to self");
                        has = true;
                        return;
                    } else if (thing.mod.inputMod) {
                        console.error("Cannot connect more than one module to input");
                        has = true;
                        return;
                    }
                    conn.end = thing;
                    conn.start.mod.Add(thing.mod);
                }
            });
        } 
    });
    if (has) {
        connections.forEach(conn => {
            if (conn.isDragging) {
                connections.splice(connections.indexOf(conn), 1);
            }
        });
    }
    connections.forEach(conn => {
        conn.isDragging = false;
    });
    
    draw();
}

function mmove(e) {
    if (!dragok) return;

    e.preventDefault();
    e.stopPropagation();

    var mx = parseInt(e.clientX - offsetX);
    var my = parseInt(e.clientY - offsetY);

    var dx = mx - startX;
    var dy = my - startY;

    if (e.buttons == 1) {
        things.forEach(thing => {
            if (thing.isDragging) {
                thing.x += dx;
                thing.y += dy;
            }
        });
    
        draw();
    
        startX = mx;
        startY = my;   
    } else if (e.buttons == 2) {
        
        connections.forEach(conn => {
            if (conn.isDragging) {
                conn.end.x = mx;
                conn.end.y = my;
            }
        });
        
        draw();

        startX = mx;
        startY = my; 
    }
}