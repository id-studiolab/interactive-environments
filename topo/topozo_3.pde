// IMPORT
import processing.video.*;
import TUIO.*;
// TRACKING SYSTEM
TuioProcessing tuioClient;

float x;
float y;
float angle;

// MOVIE VARIABLE
Movie vid0;
Movie vid1;

// WEATHER VARIABLE
String apikey = "d482a08c26995d4ea8b4cd8b789a88ef";
String url = "http://api.openweathermap.org/data/2.5/weather?q=";
String location = "Amsterdam";
String units = "metric";

String link = url + location + "&units=" + units + "&APPID=" + apikey;


JSONObject json;

int temp;
String iconurl;
PImage iconimg;

// IMAGE VARIABLE
PImage img;
PImage AMSlogo;
PImage roboatemployee;
PImage socialemployee;
PImage ring;

// FONT VARIABLE
PFont F0;
int fontsize0 = 19;
PFont F1;
int fontsize1 = 16;

// MAP DIMENSION VARIABLE
float totalradius = 1080/2;
float textspace = 100;
float mapradius = totalradius - textspace;

float buttondiameter = 60;

// SPACING
float title_space = 100 ; 
float title_angle_space = 0.18 ;
float info_space = 200 ;
float info_angle_space = 0.45 ;
float line_angle_space = 0.020 ;

//TEXT
String logo = ("AMS");
String clock;

// CITY:
float xcity0 =-170;
float ycity0 =-100;
String citytitle0 = ("Koninklijk Paleis Amsterdam");
String cityinfo0 = ("Grand residence for royal receptions since 1808.");

float xcity1 =-40;
float ycity1 =-140;
String citytitle1 = ("De Oude Kerk");
String cityinfo1 = ("13th-century church.");

float xcity2 =250;
float ycity2 =-150;
String citytitle2 = ("Nemo Science Museum");
String cityinfo2 = ("Interactive science exhibit.");

float xcity3 =380;
float ycity3 =130;
String citytitle3 = ("ARTIS");
String cityinfo3 = ("Zoo park founded in 19th century.");

// Project 1 : Roboat
float xinfo0 = -40;
float yinfo0 = -200;
float trans0A = 255;
float trans0B = 36;
String infotitle0 = ("Roboat");
String info0 = ("Autonomous boats that operates in the canal of Amsterdam.");

// Project 1 : Social Glass
float xinfo1 = 300;
float yinfo1 = -140;
float trans1A = 255;
float trans1B = 36;
String infotitle1 = ("Social Glass");
String info1 = ("Gather, analyse, visualise data generate on social media for decision makers.");

//// Project 2 : Transportation
//float xinfo2 = 10;
//float yinfo2 = -300;
//float trans2A = 255;
//float trans2B = 255;
//String infotitle2 = ("Metro");
//String info2 = ("Analyse the effects of large scale new mobility infrastructure in Amsterdam");

// FADE
float fade=0;
float change = 0.001;

// MODE
float angle_main_text = 1.1*PI; 
float angle_back = 0*PI;
//
void setup() {
  fullScreen(2);
  frameRate(24);
  smooth();

  // CREATE FONT
  F0 = createFont("MontserratBold", fontsize0, true);
  F1 = createFont("Montserrat", fontsize1, true);

  // UPLOAD VIDEO
  vid0 = new Movie(this, "roboat.mov");
  vid1 = new Movie(this, "social.mp4");
  vid0.loop();
  vid1.loop();

  // UPLOAD IMAGE
  img = loadImage("MAP.png");
  AMSlogo = loadImage("AMS_logo.png");
  roboatemployee = loadImage("R_Arjan_van_Timmeren.png");
  socialemployee = loadImage("SG_Carlo_van_der_Valk.png");
  ring = loadImage("Arrow_ring.png");
  // WEATHER AND TIME
  getweatherdata();
  gettime();
  iconimg = loadImage(iconurl);

  tuioClient = new TuioProcessing(this);
}


void draw() {
  clear();
  background(0);
  noTint();

  // TRANSLATING ----------------------------------------------------------------
  translate(width/2, height/2);


  // MAP ----------------------------------------------------------------

  map_image();
//  allignment();
  tracking();
  system();
//  citybutton();




  // BUTTONS ----------------------------------------------------------------
  //projectbuttons();
  //categorybuttons();
  //backbutton();

  // MAIN TEXT  ----------------------------------------------------------------
  main_text();
}

// ----------------------------------------------------------------

int statemode = 0;
int stateprojcategory = 1;
int statecity = 2;
int stateroboat = 3;
int statesocial = 4;
int state = statemode;

void system() {
  switch(state) {
  case 0:
    modebuttons();
    // city mode
    if (x >= (mapradius+textspace/2)*cos(angle_main_text+0.45)-buttondiameter/2 && x <= (mapradius+textspace/2)*cos(angle_main_text+0.45)+buttondiameter/2 && y >= (mapradius+textspace/2)*sin(angle_main_text+0.45)-buttondiameter/2 && y <= (mapradius+textspace/2)*sin(angle_main_text+0.45)+buttondiameter/2) {
      state = statecity;
    }
    // Project mode
    if (x >= (mapradius+textspace/2)*cos(angle_main_text-0.6)-buttondiameter/2 && x <= (mapradius+textspace/2)*cos(angle_main_text-0.6)+buttondiameter/2 && y >= (mapradius+textspace/2)*sin(angle_main_text-0.6)-buttondiameter/2 && y <= (mapradius+textspace/2)*sin(angle_main_text-0.6)+buttondiameter/2) {
      state = stateprojcategory;
    }
    break;

  case 1:
    categorybuttons();
    backbutton();
    // Mobility
    if (x >= (mapradius+textspace/2)*cos(angle_main_text+0.5*PI+0.8)-buttondiameter/2 && x <= (mapradius+textspace/2)*cos(angle_main_text+0.5*PI+0.8)+buttondiameter/2 && y >= (mapradius+textspace/2)*sin(angle_main_text+0.5*PI+0.8)-buttondiameter/2 && y <= (mapradius+textspace/2)*sin(angle_main_text+0.5*PI+0.8)+buttondiameter/2) {
      state = stateroboat;
    }
    // Social Network
    if (x >= (mapradius+textspace/2)*cos(angle_main_text+0.5*PI+0.42)-buttondiameter/2 && x <= (mapradius+textspace/2)*cos(angle_main_text+0.5*PI+0.42)+buttondiameter/2 && y >= (mapradius+textspace/2)*sin(angle_main_text+0.5*PI+0.42)-buttondiameter/2 && y <= (mapradius+textspace/2)*sin(angle_main_text+0.5*PI+0.42)+buttondiameter/2) {
      state = statesocial;
    }
    //back
    if (x >= (mapradius+textspace/2)*cos(angle_back)-buttondiameter/2 && x <= (mapradius+textspace/2)*cos(angle_back)+buttondiameter/2 && y >= (mapradius+textspace/2)*sin(angle_back)-buttondiameter/2 && y <= (mapradius+textspace/2)*sin(angle_back)+buttondiameter/2) {
      state = statemode;
    }
    break;

  case 2:
    citybutton();
    backbutton();
    city();
    //back
    if (x >= (mapradius+textspace/2)*cos(angle_back)-buttondiameter/2 && x <= (mapradius+textspace/2)*cos(angle_back)+buttondiameter/2 && y >= (mapradius+textspace/2)*sin(angle_back)-buttondiameter/2 && y <= (mapradius+textspace/2)*sin(angle_back)+buttondiameter/2) {
      state = stateprojcategory;
    }
    break;

  case 3:
    roboatbuttons();
    backbutton();
    if (x >= xinfo0-15 && x <= xinfo0+15 && y >= yinfo0-15 && y <= yinfo0+15) {
      tint(255, 127);
      image(vid0, 0, 0);
      noTint();
      infobox(infotitle0, info0, angle, xinfo0, yinfo0);
      trans0B = 255;
    }
    else  {
      trans0B = 36;
    }
    //back
    if (x >= (mapradius+textspace/2)*cos(angle_back)-buttondiameter/2 && x <= (mapradius+textspace/2)*cos(angle_back)+buttondiameter/2 && y >= (mapradius+textspace/2)*sin(angle_back)-buttondiameter/2 && y <= (mapradius+textspace/2)*sin(angle_back)+buttondiameter/2) {
      state = stateprojcategory;
    }
    break;

  case 4:  
    socialbuttons();
    backbutton();
    if (x >= xinfo1-15 && x <= xinfo1+15 && y >= yinfo1-15 && y <= yinfo1+15) {
      tint(255, 127);
      image(vid1, 0, 0);
      noTint();
      infobox(infotitle1, info1, angle, xinfo1, yinfo1);
      trans1B=255;
    }
    else  {
      trans1B = 36;
    }
    //back
    if (x >= (mapradius+textspace/2)*cos(angle_back)-buttondiameter/2 && x <= (mapradius+textspace/2)*cos(angle_back)+buttondiameter/2 && y >= (mapradius+textspace/2)*sin(angle_back)-buttondiameter/2 && y <= (mapradius+textspace/2)*sin(angle_back)+buttondiameter/2) {
      state = stateprojcategory;
    }
    break;
  }
}
void backup() {
  // Back Up ------------------------------------------------------------------------

  if (mouseX-width/2 >= xinfo0-15 && mouseX-width/2 <= xinfo0+15 && mouseY-height/2 >= yinfo0-15 && mouseY-height/2 <= yinfo0+15) {
    tint(255, 180);
    image(vid0, 0, 0);
    infobox(infotitle0, info0, PI, xinfo0, yinfo0);
    trans0A = 0;
    trans0B = 255;
  }
  if (mouseX-width/2>= xinfo1-15 && mouseX-width/2<= xinfo1+15 && mouseY-height/2 >= yinfo1-15 && mouseY-height <= yinfo1+15) {
    tint(255, 180);
    image(vid1, 0, 0);
    infobox(infotitle1, info1, 3*PI/2, xinfo1, yinfo1);
    trans1A = 0;
    trans1B = 225;
  }
}

// INFO BOX ----------------------------------------------------------------
void infobox(String infotitle, String info, Float angle, float xinfo, float yinfo) {
  strokeWeight(1.5);
  stroke(227, 36, 36);
  angle = angle - line_angle_space;
  curvewrap(infotitle, angle, fontsize0, F0, title_space, true, 1);
  angle = angle - line_angle_space;
  angle = angle - title_angle_space;
  line(mapradius*cos(angle), mapradius*sin(angle), totalradius*cos(angle), totalradius*sin(angle));
  stroke(225, 255, 0);
  line(xinfo, yinfo, mapradius*cos(angle), mapradius*sin(angle));
  angle = angle - line_angle_space;
  curvewrap(info, angle, fontsize1, F1, info_space, false, 2);
}


// CURVE TEXT WRAP ----------------------------------------------------------------

void curvewrap(String txt, Float angle, int fontsize, PFont F, float number, boolean title, int position) {
  textFont(F);
  textAlign(CENTER);

  String[] sentencelst = split(txt, ' ');        // Split the sentence into a list of words
  int k = 1;
  float arclength = 0;
  float totallength = 0;

  for (int i = 0; i < sentencelst.length; i++) {
    //Positioning of sentence
    // 1 = info title
    // 2 = info
    // 3 = middle   
    float wordradius;
    if (position == 1) {
      wordradius = totalradius - 80 + fontsize*k;
    } else if (position == 2) {
      wordradius = totalradius - textspace + 15 + fontsize*k;
    } else {
      wordradius = 500;
    }
    String word = sentencelst[i] + " ";                                         // Adding space after words
    float currentlength = textWidth(word);

    for (int j = 0; j < word.length(); j ++) {
      // Instead of a constant width, we check the width of each character.
      char currentChar = word.charAt(j);
      float w = textWidth(currentChar);

      // Each box is centered so we move half the width
      arclength += w/2;
      // Angle in radians is the arclength divided by the radius
      // Starting on the left side of the circle by adding PI
      float theta = angle - arclength / wordradius;

      pushMatrix();
      // Polar to cartesian coordinate conversion
      translate(wordradius*cos(theta), wordradius*sin(theta));
      // Rotate the box
      rotate(theta-PI/2); // rotation is offset by 90 degrees
      // Display the character
      if (title == true) {
        fill(227, 36, 36);
      } else {
        fill(0, 0, 0);
      }
      text(currentChar, 0, 0);
      popMatrix();
      // Move halfway again
      arclength += w/2;
    }

    totallength = totallength + currentlength;
    if (i < sentencelst.length - 1) {
      if (totallength + textWidth(sentencelst[i+1] + " ") > number) {
        k = k+1;
        arclength = 0;
        totallength = 0;
      }
    }
  }
}


// MOVIE LOOP ----------------------------------------------------------------

void movieEvent(Movie m) {
  m.read();
}


// WEATHER FUNCTION ----------------------------------------------------------------

void getweatherdata() {
  json = loadJSONObject(link);
  JSONObject weatherData = json.getJSONObject("main");
  temp = weatherData.getInt("temp");

  JSONArray condition = json.getJSONArray("weather");
  JSONObject number = condition.getJSONObject(0);
  String icon = number.getString("icon");
  iconurl = "http://openweathermap.org/img/w/" + icon + ".png";
}


// TIME ----------------------------------------------------------------

void gettime() {
  clock = (nf(hour(), 2) + ":" + nf(minute(), 2));
}


// TRACKING ----------------------------------------------------------------

void tracking() {
  ArrayList<TuioObject> tuioObjectList = tuioClient.getTuioObjectList();
  for (int i = 0; i<tuioObjectList.size(); i++) {
    TuioObject tobj = tuioObjectList.get(i);
    x = map(tobj.getX(), 0.2, 0.8, -1080/2, 1080/2);
    y = map(tobj.getY(), 0, 1, -1080/2, 1080/2);
    angle = tobj.getAngle();
    imageMode(CENTER);
    ring.resize(int(buttondiameter), int(buttondiameter));
    image(ring, x, y);
  }
}
void project() {
  // roboat
  if (x >= xinfo0-15 && x <= xinfo0+15 && y >= yinfo0-15 && y <= yinfo0+15) {
    tint(255, 127);
    image(vid0, 0, 0);
    infobox(infotitle0, info0, angle, xinfo0, yinfo0);
    trans0A = 0;
    trans0B = 255;
  }
  // social
  if (x >= xinfo1-15 && x <= xinfo1+15 && y >= yinfo1-15 && y <= yinfo1+15) {
    tint(255, 127);
    image(vid1, 0, 0);
    infobox(infotitle1, info1, angle, xinfo1, yinfo1);
    trans1A = 0;
    trans1B = 225;
  }
}


void map_image() {
  imageMode(CENTER);
  img.resize(int(totalradius*2), int(totalradius*2));
  image(img, 0, 0);
  AMSlogo.resize(int(0.2*562), int(0.2*207));
  tint(255, 36, 36, 255);
  rotate((angle_main_text-PI/2));
  image(AMSlogo, 0, (mapradius+textspace/2));
  rotate(-(angle_main_text-PI/2));
}

void allignment() {
  stroke(255, 0, 0);
  line(0, -1080/2, 0, 1080/2);
  line(-width/2, 0, width/2, 0);
}

void main_text() {
  if (second() == 0) {
    getweatherdata();
    //iconimg = loadImage(iconurl);
  }

  if (second() == 0) {
    gettime();
  }

  curvewrap(clock, angle_main_text+0.27, fontsize0, F0, 500, false, 3);
  curvewrap(temp + " \u00b0" + "C", angle_main_text-0.17, fontsize0, F0, 500, false, 3);
}

void roboatbuttons() {
  ellipseMode(CENTER);
  if (fade<=0) {
    change = 0.01;
  } else if (fade>=1) {
    change = -0.01;
  }
  ;
  fade += change;
  noFill();
  strokeWeight(4);
  stroke(255, trans0B, 36, map(fade, 0, 1, 50, 255));
  ellipse(xinfo0, yinfo0, buttondiameter, buttondiameter);            // Project 0 : Roboat
}

void socialbuttons() {
  ellipseMode(CENTER);
  if (fade<=0) {
    change = 0.01;
  } else if (fade>=1) {
    change = -0.01;
  }
  ;
  fade += change;
  noFill();
  strokeWeight(4);
  stroke(255, trans1B, 36, map(fade, 0, 1, 50, 255));
  ellipse(xinfo1, yinfo1, buttondiameter, buttondiameter);            // Project 1 : Social Glass
}

void modebuttons() {
  if (fade<=0) {
    change = 0.01;
  } else if (fade>=1) {
    change = -0.01;
  }
  ;
  fade += change;
  curvewrap("Project Mode", angle_main_text-0.37, fontsize0, F0, 50, false, 1);
  curvewrap("City Mode", angle_main_text+0.65, fontsize0, F0, 50, false, 1);
  noFill();
  strokeWeight(4);
  stroke(0, 0, 255, map(fade, 0, 1, 50, 255));
  ellipse((mapradius+textspace/2)*cos(angle_main_text-0.6), (mapradius+textspace/2)*sin(angle_main_text-0.6), buttondiameter, buttondiameter);
  ellipse((mapradius+textspace/2)*cos(angle_main_text+0.45), (mapradius+textspace/2)*sin(angle_main_text+0.45), buttondiameter, buttondiameter);
  strokeWeight(1);
  stroke(255, 36, 36);
  line(mapradius*cos(angle_main_text-0.32), mapradius*sin(angle_main_text-0.32), totalradius*cos(angle_main_text-0.32), totalradius*sin(angle_main_text-0.32));
  line(mapradius*cos(angle_main_text+0.33), mapradius*sin(angle_main_text+0.33), totalradius*cos(angle_main_text+0.33), totalradius*sin(angle_main_text+0.33));
}

void categorybuttons() {
  if (fade<=0) {
    change = 0.01;
  } else if (fade>=1) {
    change = -0.01;
  }
  ;
  fade += change;
  curvewrap("Mobility", angle_main_text+0.5*PI+1.05, fontsize0, F0, 50, false, 1);
  curvewrap("Social Network", angle_main_text+0.5*PI+0.65, fontsize0, F0, 50, false, 1);
  noFill();
  strokeWeight(4);
  stroke(255, 0, 0, map(fade, 0, 1, 50, 255));
  ellipse((mapradius+textspace/2)*cos(angle_main_text+0.5*PI+0.8), (mapradius+textspace/2)*sin(angle_main_text+0.5*PI+0.8), buttondiameter, buttondiameter); // Social Network
  ellipse((mapradius+textspace/2)*cos(angle_main_text+0.5*PI+0.42), (mapradius+textspace/2)*sin(angle_main_text+0.5*PI+0.42), buttondiameter, buttondiameter); // Mobility
  strokeWeight(1);
  stroke(255, 36, 36);
  line(mapradius*cos(angle_main_text+0.5*PI+0.7), mapradius*sin(angle_main_text+0.5*PI+0.7), totalradius*cos(angle_main_text+0.5*PI+0.7), totalradius*sin(angle_main_text+0.5*PI+0.7));
}

void backbutton() {
  curvewrap("Return:", angle_back+0.23, fontsize0, F0, 50, false, 3);
  noFill();
  strokeWeight(4);
  stroke(0, 0, 255);
  ellipse((mapradius+textspace/2)*cos(angle_back), (mapradius+textspace/2)*sin(angle_back), buttondiameter, buttondiameter);
}

void citybutton() {
  ellipseMode(CENTER);
  if (fade<=0) {
    change = 0.01;
  } else if (fade>=1) {
    change = -0.01;
  }
  ;
  fade += change;
  noFill();
  strokeWeight(4);
  stroke(255, 36, 36, map(fade, 0, 1, 50, 255));
  ellipse(xcity0, ycity0, buttondiameter, buttondiameter);            // Amsterdam Palace
  ellipse(xcity1, ycity1, buttondiameter, buttondiameter);            // Church
  ellipse(xcity2, ycity2, buttondiameter, buttondiameter);            // Nemo
  ellipse(xcity3, ycity3, buttondiameter, buttondiameter);            // ARTIS
}

void city() {
  // Amsterdam Palace
  if (x >= xcity0-buttondiameter/2 && x <= xcity0+buttondiameter/2 && y >= ycity0-buttondiameter/2 && y <= ycity0+buttondiameter/2) {

    infobox(citytitle0, cityinfo0, angle, xcity0, ycity0);
  }
  // Church
  if (x >= xcity1-buttondiameter/2 && x <= xcity1+buttondiameter/2 && y >= ycity1-buttondiameter/2 && y <= ycity1+buttondiameter/2) {

    infobox(citytitle1, cityinfo1, angle, xcity1, ycity1);
  }
  // Nemo
  if (x >= xcity2-buttondiameter/2 && x <= xcity2+buttondiameter/2 && y >= ycity2-buttondiameter/2 && y <= ycity2+buttondiameter/2) {

    infobox(citytitle2, cityinfo2, angle, xcity2, ycity2);
  }
  // Artis
  if (x >= xcity3-buttondiameter/2 && x <= xcity3+buttondiameter/2 && y >= ycity3-buttondiameter/2 && y <= ycity3+buttondiameter/2) {

    infobox(citytitle3, cityinfo3, angle, xcity3, ycity3);
  }
}
