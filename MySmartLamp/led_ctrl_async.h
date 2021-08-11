#ifndef led_ctrl_async_h
#define led_ctrl_async_h

class led_ctrl{
  public:
    led_ctrl( int p );
    bool _debug = false;

    void set_pattern( int a[] );
    void set_delay( int d );
    
    void start( );
    
    void blink( );
    
    void high( );
    void low( );
    void opposite( );

    int status();
  private:
    int LED;
    int state = LOW;
    int *pattern;
    int p_index = 0;
    int p_length = 0;
    int _delay = 50;
    unsigned long _start_time;
};

#endif
