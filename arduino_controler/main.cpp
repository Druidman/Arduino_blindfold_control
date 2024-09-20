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

enum State{
  Open,
  Closed,
  Somewhere
};

enum Action{
  Opening,
  Closing,
  Stop
};


typedef struct BlindFold BlindFold;
typedef enum Room Room;
typedef enum Action Action;
typedef enum State State;


struct BlindFold {
  Room room; // room
  int decision_pin; // on and off pin
  int motor_pin; // up or down pin

  const int close_time; // time it takes to close
  const int open_time;// time it takes to open

  int start_time; // start time of opening or closing
  int end_time; //end time of opening or closing
  int stop_time;

  int time_to_open = open_time;
  int time_to_close = close_time;


  

  
  
  Action action = Stop; //what type of action is blindfold doing
  Action previous_action = Stop;
  State state = Open;

  

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
    double ratio;
    ratio = open_time/ (double) close_time;

    int prev_end_time = end_time;

    switch(action){

      case Opening:
        switch(previous_action){
          case Opening:
            end_time = start_time + prev_end_time - stop_time;
            break;
          case Closing:
            end_time = start_time + (open_time - ((prev_end_time - stop_time) * ratio));
            break;
          default:
            end_time = start_time + open_time;
            break;
        }
        break;

      case Closing:
        switch(previous_action){
          case Opening:
            end_time = start_time + (close_time - ((prev_end_time - stop_time) / ratio));
            break;
          case Closing:
            end_time = start_time + prev_end_time - stop_time;
            break;
          default:
            end_time = start_time + close_time;
            break;
        }
        break;

      default:
        break;
    }
  }

  void open() {
    if (state == Open){
      Serial.println("Blindfold already opened");
      return;
    }
    else if(action != Stop){
      Serial.println("already working");
      return;
    }
    
    digitalWrite(decision_pin, HIGH);
    digitalWrite(motor_pin, HIGH);
    action = Opening;
    state = Somewhere;

    start_time = millis();
    calculate_end_time();

    

  }

  void close() {
    if (state == Closed){
      Serial.println("Blindfold already closed");
      return;
    }
    else if(action != Stop){
      Serial.println("already working");
      return;
    }
    
    digitalWrite(decision_pin, HIGH);
    digitalWrite(motor_pin, LOW);
    
    action = Closing;
    
    state = Somewhere;

    start_time = millis();
    calculate_end_time();

    
  }

  void stop(){
    if (state == Open || state == Closed || action==Stop ){
      Serial.println("Not moving rn");
      return;
    }
    
    digitalWrite(decision_pin, LOW);

    state = Somewhere;
    previous_action = action;
    action = Stop;

    stop_time = millis()-start_time;

    
    
    Serial.println("time of action elapsed: ");
    Serial.println(stop_time);
    
  }

  void check(){
    if (state != Somewhere || action == Stop){
      return;
    };

    int current_time = millis();

    switch(action){
      case Opening:
  
        if (current_time >= end_time){
          digitalWrite(decision_pin,LOW);
          Serial.println("Opened");
          action = Stop;
          state = Open;
          stop_time = 0;
        }

        break;
      case Closing:
        
        if (current_time >= end_time){
          digitalWrite(decision_pin,LOW);
          Serial.println("Closed");
          action = Stop;
          state = Closed;
          stop_time = 0;
        }
        break;

      default:
        Serial.println("None");
        break;

    }

  }
};

String read_serial();
void check_blind_folds();

BlindFold blindfolds[] = {
  BlindFold(Mateusz_room, 53, 51, 16500, 18000)

  // BlindFold(Kitchen, 53, 51, 16500, 18000),
  // BlindFold(Kitchen, 53, 51, 16500, 18000),

  // BlindFold(main_room, 53, 51, 16500, 18000),
  // BlindFold(main_room, 53, 51, 16500, 18000),
  // BlindFold(main_room, 53, 51, 16500, 18000),

  // BlindFold(guest_room, 53, 51, 16500, 18000),

  // BlindFold(top_corridor, 53, 51, 16500, 18000),

  // BlindFold(Dad_room, 53, 51, 16500, 18000),
  // BlindFold(Dad_room, 53, 51, 16500, 18000),

  // BlindFold(Parents_room, 53, 51, 16500, 18000),
  // BlindFold(Parents_room, 53, 51, 16500, 18000),
  // BlindFold(Parents_room, 53, 51, 16500, 18000),

  // BlindFold(Natalia_room, 53, 51, 16500, 18000),
  // BlindFold(Natalia_room, 53, 51, 16500, 18000),

  // BlindFold(Gosia_room, 53, 51, 16500, 18000),
  // BlindFold(Gosia_room, 53, 51, 16500, 18000),

  // BlindFold(Ania_room, 53, 51, 16500, 18000),
  // BlindFold(Ania_room, 53, 51, 16500, 18000)

  // BlindFold(ground_storage_room, 53, 51, 16500, 18000),

  // BlindFold(top_storage_room, 53, 51, 16500, 18000),
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  int message = Serial.read();

  check_blind_folds();


  

  if (message == 49){

    Serial.println("closing");
    blindfolds[0].close();
    
  }
  else if (message == 48){
    Serial.println("stopping");
    blindfolds[0].stop();
  }
  else if (message == 50){
    Serial.println("opening");
    blindfolds[0].open();
    
  };
  
 
}
void check_blind_folds(){
  int length = sizeof(blindfolds) / sizeof(blindfolds[0]);
  for (int i=0; i<length; i++){
    blindfolds[i].check();
  }
}

String read_serial(){
  int bytereceived = Serial.read();
  String message;
  char length = bytereceived;
  int ilength = length;
   
  do{
    
    
    
    if (bytereceived != 10 && bytereceived != -1){
      char letter = bytereceived - 32;
      message += letter;
      
    }
  
  }while (message.length() < ilength);

  if (message){
    return message;
  }
  else{
    return "";
  }
}