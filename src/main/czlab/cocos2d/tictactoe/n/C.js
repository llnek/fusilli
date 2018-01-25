/*Auto generated by Kirby v1.0.0 - Thu Jan 25 2018 02:47:41 GMT-0800 (PST)
  czlab.fusilli.ttt.C
{"doc":"","author":"Kenneth Leung"}
*/

const k= require("kirby");
const conj_BANG=k["conj_BANG"];
const opt_QMRK__QMRK=k["opt_QMRK__QMRK"];
const merge=k["merge"];
const kirbystdlibref= require("kirby");
const __module_namespace__= "czlab.fusilli.ttt.C";
const BD_SZ=3;
const GD_SZ=(BD_SZ * BD_SZ);
const ArrCells=function () {
let ____args=Array.prototype.slice.call(arguments);
return (function() {
let ret=[];
for (let i=0,____break=false;((!____break) && (i < GD_SZ)); i=(i + 1)){
ret.push(0);
}
return ret;
}).call(this);
};
const ArrDim=function () {
let ____args=Array.prototype.slice.call(arguments);
return (function() {
let ret=[];
for (let i=0,____break=false;((!____break) && (i < BD_SZ)); i=(i + 1)){
ret.push(0);
}
return ret;
}).call(this);
};
//Stores the grid values and the set of winning goals
class Grid extends ecs.Component {
////////////////////////////////////////////////////////////////////////////////
//fn: [constructor] in file: C.ky, line: 21
constructor (seed) {
(this["goals"]=mapGoalSpace(),this["tpid"]="czlab.fusilli.ttt.C/Grid",this["vals"]=Array.prototype.slice.call(seed));
return this;
};
}
class Player extends ecs.CStats {
////////////////////////////////////////////////////////////////////////////////
//fn: [constructor] in file: C.ky, line: 30
constructor (category,value,id) {
super(value);
(this["tpid"]="czlab.fusilli.ttt.C/Player",this["color"]="",this["pidlong"]="",this["pid"]="",this["id"]=0,this["category"]=category,this["pnum"]=id);
return this;
};
}
class Players extends ecs.Component {
////////////////////////////////////////////////////////////////////////////////
//fn: [constructor] in file: C.ky, line: 41
constructor () {
(this["parr"]=[ null, null, null ],this["tpid"]="czlab.fusilli.ttt.C/Players");
return this;
};
}
class CSquare extends ecs.CPixie {
////////////////////////////////////////////////////////////////////////////////
//fn: [constructor] in file: C.ky, line: 49
constructor (cell) {
(this["cell"]=cell,this["png"]="z",this["value"]=0);
return this;
};
////////////////////////////////////////////////////////////////////////////////
//fn: [create] in file: C.ky, line: 52
static create (cell) {
return (function() {
let GS__3=new CSquare(cell);
GS__3.initWithSpriteFrameName("z.png");
return GS__3;
}).call(this);
};
////////////////////////////////////////////////////////////////////////////////
//fn: [toggle] in file: C.ky, line: 56
toggle (nv) {
let x=XConfig.cstVal("CV_X");
let o=XConfig.cstVal("CV_O");
return ((this.value === 0) ?
(function() {
if ((nv === x)) {
(this["png"]="x");
};
if ((nv === o)) {
(this["png"]="o");
};
(this["value"]=nv);
return this.setSpriteFrame([ png, ".png" ].join(""));
}).call(this) :
null);
};
////////////////////////////////////////////////////////////////////////////////
//fn: [flip] in file: C.ky, line: 65
flip () {
return ((this.value !== 0) ?
this.setSpriteFrame([ png, ".i.png" ].join("")) :
null);
};
}
class CSquares extends ecs.Component {
////////////////////////////////////////////////////////////////////////////////
//fn: [constructor] in file: C.ky, line: 72
constructor () {
(this["boxes"]=[],this["sqs"]=[],this["tpid"]="czlab.fusilli.ttt.C/CSquares");
return this;
};
}
class CellPos extends ecs.Component {
////////////////////////////////////////////////////////////////////////////////
//fn: [constructor] in file: C.ky, line: 80
constructor () {
(this["cell"]=-1,this["px"]=-1,this["py"]=-1,this["tpid"]="czlab.fusilli.ttt.C/CellPos");
return this;
};
}
class GVars extends ecs.Component {
////////////////////////////////////////////////////////////////////////////////
//fn: [constructor] in file: C.ky, line: 86
constructor () {
(this["pnum"]=-1,this["lastWinner"]=-1,this["tpid"]="czlab.fusilli.ttt.C/GVars");
return this;
};
}
////////////////////////////////////////////////////////////////////////////////
//fn: [fillArray] in file: C.ky, line: 90
const fillArray = function (cnt,func) {
let ret=[];
for (let x=0,GS__4=cnt,____break=false;((!____break) && (x < GS__4)); x=(x + 1)){
conj_BANG(ret,func(x));
}
return ret;
};
////////////////////////////////////////////////////////////////////////////////
//fn: [mapGridPos] in file: C.ky, line: 96
//Calculate position of each individual cells in the grid,
//so that we can detect when a user clicks on the cell
const mapGridPos = function () {
let GS__5=Array.prototype.slice.call(arguments,0);
let scale=GS__5[0];
(scale=opt_QMRK__QMRK(scale,1));
let csz=cx.scaleSize(cx.calcSize("z.png"),scale);
let ro=((8 / 72) * scale);
let gh=(csz.height * ro);
let gw=(csz.width * ro);
let zh=((csz.height * BD_SZ) + (gh * (BD_SZ - 1)));
let zw=((csz.width * BD_SZ) + (gw * (BD_SZ - 1)));
let boxes=fillArray(GD_SZ,function () {
let ____args=Array.prototype.slice.call(arguments);
return new ecs.Box4();
});
let wb=cx.visBox();
let x0=(wb.cx - HTV(zw));
let y0=(wb.cy + HTV(zh));
let x1=x0;
let y1=y0;
let bx,x2,y2;
for (let r=0,____break=false;((!____break) && (r < BD_SZ)); r=(r + 1)){
for (let c=0,____break=false;((!____break) && (c < BD_SZ)); c=(c + 1)){
(bx=kirbystdlibref.getProp(boxes,(c + (r * BD_SZ))),y2=(y1 - csz.height),x2=(x1 + csz.width));
(bx["top"]=y1,bx["right"]=x2,bx["bottom"]=y2,bx["left"]=x1);
(x1=(x2 + gw));
}
(y1=(y2 - gh),x1=x0);
}
return boxes;
};
////////////////////////////////////////////////////////////////////////////////
//fn: [mapGoalSpace] in file: C.ky, line: 126
const mapGoalSpace = function () {
let dx=ArrDim();
let dy=ArrDim();
let goals=[ dx, dy ];
let h,v;
for (let r=0,____break=false;((!____break) && (r < BD_SZ)); r=(r + 1)){
(h=ArrDim(),v=ArrDim());
for (let c=0,____break=false;((!____break) && (c < BD_SZ)); c=(c + 1)){
(h[c]=(c + (r * BD_SZ)));
(v[c]=(r + (c * BD_SZ)));
}
conj_BANG(goals,h,v);
(dx[r]=(r + (r * BD_SZ)));
(dy[r]=((BD_SZ - r - 1) * (BD_SZ + r)));
}
return goals;
};
////////////////////////////////////////////////////////////////////////////////
//fn: [fmtGameData] in file: C.ky, line: 139
const fmtGameData = function (mode) {
return ((mode === ecs.GMode.TWO) ?
{ "ppids": { "p1": [ 1, "player1" ], "p2": [ 2, "player2" ] }, "pnum": 1 } :
((mode === ecs.GMode.ONE) ?
{ "ppids": { "cpu": [ 2, "computer" ], "p1": [ 1, "player1" ] }, "pnum": 1 } :
((mode === ecs.GMode.NET) ?
{ "ppids": {}, "pnum": 0 } :
(true ?
(function (){ throw new Error([ "Bad game mode: ", mode ].join(""));}).call(this) :
null))));
};
module.exports = {
da57bc0172fb42438a11e6e8778f36fb: { ns: "czlab.fusilli.ttt.C", macros: {} },
BD_SZ:BD_SZ,
GD_SZ:GD_SZ,
ArrCells:ArrCells,
ArrDim:ArrDim,
Grid:Grid,
Player:Player,
Players:Players,
CSquare:CSquare,
CSquares:CSquares,
CellPos:CellPos,
GVars:GVars,
fillArray:fillArray,
mapGridPos:mapGridPos,
mapGoalSpace:mapGoalSpace,
fmtGameData:fmtGameData
};