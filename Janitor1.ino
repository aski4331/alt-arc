const int faucetPin = A0;
int faucetVal = 0;


const int lightPin = A1;
int lightVal = 0;


const int swatPin = 2;
int swatVal = 0;

const int sprayPin = 3;
int sprayVal = 0;

const int toiletPin = 4;
int toiletVal = 0;


const int ventPin = 5;
int ventVal = 0;


//after losing a life, give more time to complete again


//Tracking variables:

int activeTask = -1;


bool override = true;


int health = 3; //number of failures allowed


int difficulty = 100; //subtracted from frequency after each task completed

long unsigned frequency = 4000; //how often tasks get assigned in milliseconds

long unsigned limit = 1000; //lower limit to frequency

long unsigned taskTime = 7000; //time given to complete a task



int stage = 0; //0 welcome, 1 game, 2 death screen


//Timekeepers:
long toiletT = 0; 
long ventT = 0; 
long swatT = 0; 
long sprayT = 0; 
long faucetT = 0; 
long lightT = 0; 

long T = 0;

//Taskkeepers
bool toilet = false; 
bool vent = false; 
bool swat = false; 
bool spray = false; 
bool faucet = false; 
bool light = false; 


//Task specific

bool open_close = true; //vent open true or closed false

int threshold = 900; //threshold for photocell

int faucetThresh = 10; //threshold for potentiometer (faucet)
int faucetRand = 0; //random faucet value

int plunges = 0; //tracks plunging multiple times
int randPlunges = 5;
bool plungeOK = true;

int sprays = 0; //tracks spraying mulitple times
int randSprays = 5;
bool sprayOK = true;

int swats = 0; //tracks swatting multiple times
int randSwats = 5;
bool swatOK = true;





void setup() {
  // put your setup code here, to run once:

 pinMode(ventPin, INPUT_PULLUP);
 pinMode(swatPin, INPUT_PULLUP);
 pinMode(toiletPin, INPUT_PULLUP);
 pinMode(lightPin, INPUT);
 pinMode(sprayPin, INPUT_PULLUP);
 pinMode(faucetPin, INPUT);

   Serial.begin(9600);

   randomSeed(analogRead(A2));

   

}

void loop() {

  swatVal = digitalRead(swatPin); Serial.print(swatVal); Serial.print(",");              //0
  lightVal = analogRead(lightPin); Serial.print(lightVal); Serial.print(",");           //1
  toiletVal = digitalRead(toiletPin); Serial.print(toiletVal); Serial.print(",");        //2
  ventVal = digitalRead(ventPin); Serial.print(ventVal); Serial.print(",");             //3
  sprayVal = digitalRead(sprayPin); Serial.print(sprayVal); Serial.print(",");           //4            
  faucetVal = analogRead(faucetPin); Serial.print(faucetVal);    Serial.print(",");                    //5


  Serial.print(randSwats); Serial.print(",");
  Serial.print(swats); Serial.println(",");


if (stage == 0)
{

//correct the vent
  if (ventVal == LOW && open_close == false)
  {
    open_close = true;
    Keyboard.print("l");
  }
  else if (ventVal == HIGH && open_close == true)
  {
    open_close = false;
    Keyboard.print("j");
  }

  if (toiletVal == LOW) //plunge to start
  {
    Keyboard.print("y");
    stage = 1;
  }

}

if (stage == 2)
{
  //Keyboard.print("x"); //death screen
  if (swatVal == LOW)                                   //RESET
  {
    
    frequency = 4000;
    
      //Timekeepers:
      toiletT = 0; 
      ventT = 0; 
      swatT = 0; 
      sprayT = 0; 
      faucetT = 0; 
      lightT = 0; 

      T = 0;

      //Taskkeepers
      toilet = false; 
      vent = false; 
      swat = false; 
      spray = false; 
      faucet = false; 
      light = false; 

      activeTask = -1;

      override = true;

      health = 3; //number of failures allowed

    if (ventVal == LOW && open_close == false)
    {
      open_close = true;
      Keyboard.print("l");
    }
    else if (ventVal == HIGH && open_close == true)
    {
      open_close = false;
      Keyboard.print("j");
    }





    stage = 0;
    Keyboard.print("w");


  }
}

  if(health == 0)
  {
    stage = 2;
  }


if (stage == 1)
{



if (millis() - T >= frequency || override)
{
  override = false;
  activeTask = random(1,7); //pick a new task to activate 1,7
  switch(activeTask)
  {
    case 1: //plunge toilet
      if (!toilet)
      {
        toilet = true;
        randPlunges = random(1,5);
        toiletT = millis(); //reset time counter
        Keyboard.print("a"); //press a
      }
      else
      {
        override = true;
      }
    break;

    case 2: //swat bugs
      if (!swat)
      {
        swat = true;
        randSwats = random(1,5);
        swatT = millis(); //reset time counter
        Keyboard.print("e"); //press a
      }
      else
      {
        override = true;
      }
    break;

    case 3: //open/close vent
      if (!vent)
      {
        ventT = millis(); //reset time counter
        vent = true;
        if (open_close == true) //open
        {Keyboard.print("i"); }//press i - close the vent!
        else // (open/close == false) closed
        {Keyboard.print("k"); }//press k - open the vent!
        
      }
      else
      {
        override = true;
      }
    break;

    case 4: //turn on light
      if (!light)
      {
        light = true;
        lightT = millis(); //reset time counter
        Keyboard.print("c"); //press c
      }
      else
      {
        override = true;
      }
    break;

    case 5: //spray rats
      if (!spray)
      {
        spray = true;
        randSprays = random(1,6);
        sprayT = millis(); //reset time counter
        Keyboard.print("g"); //press g
      }
      else
      {
        override = true;
      }
    break;

    case 6: //turn off faucet
      if (!faucet)
      {
        faucet = true;
        faucetRand = random(10,1015); //get a random value
        faucetT = millis(); //reset time counter
        Keyboard.print("m"); //press m
      }
      else
      {
        override = true;
      }
    break;
  }

  T = millis();
}//end task activation


//task checker

  if (toilet) //toilet task is active
  {

    if ((millis() - toiletT) > taskTime) //check if over time
    {
      //toilet = false;
      //Keyboard.print("b"); //press b

      health = health - 1; 
      Keyboard.print("z"); //press z

      toiletT = millis();

    }
    else //still have time
    {
      if (toiletVal == LOW && plunges < randPlunges && plungeOK)
      {
        plunges++;
        plungeOK = false;
      }
      if(toiletVal == HIGH)
      {
        plungeOK = true;
      }
      else if(toiletVal == LOW && plunges == randPlunges) //if task gets completed
      {
        plunges = 0;
        toilet = false;
        Keyboard.print("b"); //press b

        toiletT = 0;

        if (frequency > limit)
        {
          frequency = frequency - difficulty;
        }
      }
    }
        
  }

  if (swat) //swat flies task is active
  {

    if ((millis() - swatT) > taskTime) //check if over time
    {
      //swat = false;
      //Keyboard.print("f"); //press f

      health = health - 1; 
      Keyboard.print("z"); //press z

      swatT = millis();

    }
    else //still have time
    {
      if (swatVal == LOW && swats < randSwats && swatOK)
      {
        swats++;
        swatOK = false;
      }
      if(swatVal == HIGH)
      {
        swatOK = true;
      }
      else if(swatVal == LOW && swats == randSwats) //if task gets completed
      {
        swats = 0;
        swat = false;
        Keyboard.print("f"); //press f

        swatT = 0;

        
        if (frequency > limit)
        {
          frequency = frequency - difficulty;
        }

      }
    }
        
  }

  if (vent) //vent task is active
  {

    if ((millis() - ventT) > taskTime) //check if over time
    {
      // vent = false;
      // if (open_close == true) //open
      // {Keyboard.print("j");} //press j
      // else
      // {Keyboard.print("l");} //press l


      health = health - 1; 
      Keyboard.print("z"); //press z

      ventT = millis();

    }
    else //still have time
    {
      if(open_close == true && ventVal == HIGH) //if task gets completed
      {
        vent = false;
        open_close = false; //vent is now closed
        Keyboard.print("j"); //press j

        ventT = 0;

        
        if (frequency > limit)
        {
          frequency = frequency - difficulty;
        }

      }
      else if(open_close == false && ventVal == LOW) //if task gets completed
      {
        vent = false;
        open_close = true;
        Keyboard.print("l"); //press l

        ventT = 0;

        
        if (frequency > limit)
        {
          frequency = frequency - difficulty;
        }

      }
    }
        
  }


  if (light) //light task is active
  {

    if ((millis() - lightT) > taskTime) //check if over time
    {
      //light = false;
      //Keyboard.print("d"); //press d

      health = health - 1; 
      Keyboard.print("z"); //press z

      lightT = millis();

    }
    else //still have time
    {
      if(lightVal >= threshold) //if task gets completed
      {
        light = false;
        Keyboard.print("d"); //press d

        lightT = 0;

        
        if (frequency > limit)
        {
          frequency = frequency - difficulty;
        }

      }
    }
        
  }


  if (spray) //spray rats task is active
  {

    if ((millis() - sprayT) > taskTime) //check if over time
    {
      //spray = false;
      //Keyboard.print("h"); //press d

      health = health - 1; 
      Keyboard.print("z"); //press z

      sprayT = millis();

    }
    else //still have time
    {

      if (sprayVal == LOW && sprays < randSprays && sprayOK)
      {
        sprays++;
        sprayOK = false;
      }
      if(sprayVal == HIGH)
      {
        sprayOK = true;
      }
      else if(sprayVal == LOW && sprays == randSprays) //if task gets completed
      {
        sprays = 0;
        spray = false;
        Keyboard.print("h"); //press d

        sprayT = 0;

        
        if (frequency > limit)
        {
          frequency = frequency - difficulty;
        }

      }
    }
        
  }



//m is need to turn the faucet
//n is you did it
    if (faucet) //faucet task is active
  {

    if ((millis() - faucetT) > taskTime) //check if over time
    {
      //faucet = false;
      //Keyboard.print("n"); //press n

      health = health - 1; 
      Keyboard.print("z"); //press z

      faucetT = millis();

    }
    else //still have time
    {
      if(faucetVal >= (faucetRand - faucetThresh) && faucetVal <= (faucetRand + faucetThresh)) //if task gets completed
      {
        faucet = false;
        Keyboard.print("n"); //press d

        faucetT = 0;

        
        if (frequency > limit)
        {
          frequency = frequency - difficulty;
        }

      }
    }
        
  }

}//end stage 1, game
  



  

} //end loop
