ig.module( 'demos.levels.demo-controls' )
.requires( 'impact.image','game.entities.player' )
.defines(function(){
LevelDemoControls=/*JSON[*/{"entities":[{"type":"EntityPlayer","x":552,"y":530}],"layer":[{"name":"main","width":10,"height":10,"linkWithCollision":false,"visible":1,"tilesetName":"media/sprites/cave-tiles-a.png","repeat":false,"preRender":true,"distance":"1","tilesize":100,"foreground":false,"data":[[6,6,6,6,6,6,6,6,6,6],[6,7,7,7,7,7,7,7,7,6],[6,7,7,7,7,7,7,7,7,6],[6,7,6,6,6,7,7,7,7,6],[6,7,7,7,7,7,7,7,7,6],[6,7,7,7,7,7,7,7,7,6],[6,6,6,7,7,6,6,6,6,6],[6,7,7,7,7,7,7,7,7,6],[6,7,7,7,7,7,7,7,7,6],[6,6,6,6,6,6,6,6,6,6]]},{"name":"collision","width":10,"height":10,"linkWithCollision":false,"visible":1,"tilesetName":"","repeat":false,"preRender":false,"distance":1,"tilesize":100,"foreground":false,"data":[[1,1,1,1,1,1,1,1,1,1],[1,0,0,0,0,0,0,0,0,1],[1,0,0,0,0,0,0,0,0,1],[1,0,1,1,1,0,0,0,0,1],[1,0,0,0,0,0,0,0,0,1],[1,0,0,0,0,0,0,0,0,1],[1,1,1,0,0,1,1,1,1,1],[1,0,0,0,0,0,0,0,0,1],[1,0,0,0,0,0,0,0,0,1],[1,1,1,1,1,1,1,1,1,1]]}]}/*]JSON*/;
LevelDemoControlsResources=[new ig.Image('media/sprites/cave-tiles-a.png')];
});