ig.module( 'game.levels.5' )
.requires( 'impact.image','game.entities.player','game.entities.exit','game.entities.block' )
.defines(function(){
Level5=/*JSON[*/{"entities":[{"type":"EntityPlayer","x":336,"y":226},{"type":"EntityExit","x":612,"y":8},{"type":"EntityBlock","x":712,"y":500,"settings":{"spriteId":2}},{"type":"EntityBlock","x":204,"y":200},{"type":"EntityBlock","x":404,"y":200,"settings":{"spriteId":1}},{"type":"EntityBlock","x":424,"y":800,"settings":{"spriteId":3}}],"layer":[{"name":"main","width":10,"height":10,"linkWithCollision":false,"visible":1,"tilesetName":"media/sprites/cave-tiles-a.png","repeat":false,"preRender":true,"distance":"1","tilesize":100,"foreground":false,"data":[[6,6,6,6,6,6,7,6,6,6],[6,7,7,7,7,7,7,7,7,6],[6,7,7,7,7,7,7,7,7,6],[6,7,6,6,6,7,7,7,7,6],[6,7,7,7,7,7,7,7,7,6],[6,7,7,7,7,7,7,7,7,6],[6,6,6,7,7,6,6,6,6,6],[6,7,7,7,7,7,7,7,7,6],[6,7,7,7,7,7,7,7,7,6],[6,6,6,6,6,6,6,6,6,6]]},{"name":"collision","width":10,"height":10,"linkWithCollision":false,"visible":1,"tilesetName":"","repeat":false,"preRender":false,"distance":1,"tilesize":100,"foreground":false,"data":[[1,1,1,1,1,1,0,1,1,1],[1,0,0,0,0,0,0,0,0,1],[1,0,0,0,0,0,0,0,0,1],[1,0,1,1,1,0,0,0,0,1],[1,0,0,0,0,0,0,0,0,1],[1,0,0,0,0,0,0,0,0,1],[1,1,1,0,0,1,1,1,1,1],[1,0,0,0,0,0,0,0,0,1],[1,0,0,0,0,0,0,0,0,1],[1,1,1,1,1,1,1,1,1,1]]}]}/*]JSON*/;
Level5Resources=[new ig.Image('media/sprites/cave-tiles-a.png')];
});