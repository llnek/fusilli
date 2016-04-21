ig.module( 'game.levels.3' )
.requires( 'impact.image','game.entities.player','game.entities.alien-d','game.entities.exit' )
.defines(function(){
Level3=/*JSON[*/{"entities":[{"type":"EntityPlayer","x":264,"y":230},{"type":"EntityAlienD","x":488,"y":100,"settings":{"spriteId":2}},{"type":"EntityAlienD","x":152,"y":100},{"type":"EntityAlienD","x":328,"y":100,"settings":{"spriteId":1}},{"type":"EntityAlienD","x":780,"y":100,"settings":{"spriteId":4}},{"type":"EntityAlienD","x":648,"y":100,"settings":{"spriteId":3}},{"type":"EntityExit","x":1008,"y":8}],"layer":[{"name":"main","width":12,"height":5,"linkWithCollision":false,"visible":1,"tilesetName":"media/sprites/cave-tiles-a.png","repeat":false,"preRender":true,"distance":"1","tilesize":100,"foreground":false,"data":[[6,6,6,6,6,6,6,6,6,6,7,6],[6,7,7,7,7,7,7,7,7,7,7,6],[6,7,7,7,7,7,7,7,7,7,7,6],[6,6,6,6,6,6,6,6,6,6,6,6],[6,7,7,7,7,7,7,7,7,7,7,6]]},{"name":"collision","width":12,"height":5,"linkWithCollision":false,"visible":1,"tilesetName":"","repeat":false,"preRender":false,"distance":1,"tilesize":100,"foreground":false,"data":[[1,1,1,1,1,1,1,1,1,1,0,1],[1,0,0,0,0,0,0,0,0,0,0,1],[1,0,0,0,0,0,0,0,0,0,0,1],[1,1,1,1,1,1,1,1,1,1,1,1],[1,0,0,0,0,0,0,0,0,0,0,1]]}]}/*]JSON*/;
Level3Resources=[new ig.Image('media/sprites/cave-tiles-a.png')];
});