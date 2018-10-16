function setup() {
  createCanvas(400, 400);
}

function draw() {
  background(220);
  background(220,150,150);
  
  stroke('#111');
  strokeWeight(10);
  
  line(width/2,height/2,mouseX,mouseY);
  rectMode(CENTER);
  fill(255);
  rect(mouseX,mouseY,50,50);
  
  ellipse(width/2,height/2,20,20);
  
  for (var i=0; i<15; i++){
    noFill();
  	rect(width/2,height/2,i*30,i*30)
  }
}