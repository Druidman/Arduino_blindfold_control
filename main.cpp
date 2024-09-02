enum Room{
  Kitchen,
  main_room,
  guest_room,
  ground_storage_room,
  top_storage_room,
  top_corridor,
  Dad_room,
  Parents_room,
  Mateusz_room,
  Natalia_room,
  Gosia_room,
  Ania_room
};

enum Action{
  Opening,
  Closing,
  Stop
  };


typedef struct BlindFold BlindFold;
typedef enum Room Room;
typedef enum Action Action;




struct BlindFold {
  Room room; // room
  int decision_pin; // on and off pin
  int motor_pin; // up or down pin

  int start_time; // start time of opening or closing
  int end_time; //end time of opening or closing
  int stop_time = 0; //time when blind fold was stopped

  int close_time; // time it takes to close
  int open_time;// time it takes to open

  bool working = false; //whether blin fold is stationary or moving
  Action action = Stop; //what type of action is blindfold doing
  Action previous_action = Stop;

  

  BlindFold(Room room, int decision_pin, int motor_pin, int close_time,int open_time)
    : 
    room(room), 
    decision_pin(decision_pin), 
    motor_pin(motor_pin),
    close_time(close_time), 
    open_time(open_time){
    pinMode(decision_pin, OUTPUT);
    pinMode(motor_pin, OUTPUT);
  }
  void calculate_end_time(){
    double open_to_close;
    open_to_close = open_time/ (double) close_time;
    Serial.println("Open to close time ratio: ");
    Serial.println(open_to_close);


    if (stop_time == 0){
      switch(action){

        case Opening:
          end_time = start_time + open_time;
          Serial.println("Open normal end time: ");
          Serial.println(end_time-millis());
          break;
          
        case Closing:
          end_time = start_time + close_time;
          Serial.println("Close normal end time: ");
          Serial.println(end_time-millis());
          break;

        default:
          break;

      }
      stop_time = 0;
      return;
    }
    
    switch(action){

      case Opening:

        switch(previous_action){

          case Opening:
            end_time = start_time + open_time - stop_time;
            Serial.println("Open Open end time: ");
            Serial.println(end_time-millis());
            break;

          case Closing:
            int calc = (int)(stop_time*open_to_close + 0.5);
            end_time = start_time + calc;
            Serial.println("Open Close end time: ");
            Serial.println(end_time-millis());
            break;

          default:
            break;
        }

        break;

      case Closing:
        Serial.println(action);
        Serial.println(previous_action);

        switch(previous_action){

          case Opening:
            int calc = (int)(stop_time/open_to_close + 0.5);
            end_time = start_time + calc;
            Serial.println("Close Open end time: ");
            Serial.println(end_time-millis());
            break;

          case Closing:
            end_time = start_time + close_time - stop_time;
            Serial.println("Close Close end time: ");
            Serial.println(end_time-millis());
            break;

          default:
            break;
        }

        break;

      default:

        break;
    }

    stop_time = 0;
  }
  void open() {
    if (working){
      Serial.println("already working");
      return;
    }
    
    digitalWrite(decision_pin, HIGH);
    digitalWrite(motor_pin, HIGH);
    action = Opening;
    working = true;

    start_time = millis();
    calculate_end_time();

    

  }

  void close() {
    if (working){
      Serial.println("already working");
      return;
    }
    
    digitalWrite(decision_pin, HIGH);
    digitalWrite(motor_pin, LOW);
    
    action = Closing;
    working = true;

    start_time = millis();
    calculate_end_time();

    
  }

  void stop(){
    if (!working){
      Serial.println("Not moving rn");
      return;
    }
    digitalWrite(decision_pin, LOW);

    working = false;
    previous_action = action;
    action = Stop;

    int current_time = millis();

    stop_time = current_time - start_time;
    Serial.println("time of action elapsed: ");
    Serial.println(stop_time);
    
  }

  void check(){
    if (!working){
      return;
    };

    int current_time = millis();

    switch(action){
      case Opening:
        
        
        

        if (current_time >= end_time){
          digitalWrite(decision_pin,LOW);
          action = Stop;
          working = false;
        }

        break;
      case Closing:
        
        
        
        if (current_time >= end_time){
          digitalWrite(decision_pin,LOW);
          action = Stop;
          working = false;
        }
        break;

      default:
        Serial.println("None");
        break;

    }

  }
};


BlindFold blind(Kitchen, 53, 51, 1000, 1100);
String read_serial();

void setup() {
  Serial.begin(9600);

  
}

void loop() {
  int message = Serial.read();

  

  if (message == 49){
    blind.close();
  }
  else if (message == 48){
    blind.stop();
  }
  else if (message == 50){
    blind.open();
  };
  

  blind.check();
 
  
}