


#define BLANK 10

esp_err_t i2cinit(void);

void led_cmd(uint8_t comd);

void ssd1306_init(void);

void clearEntireDisplay(void);

void set_Cursor(int x, int y);

void led_data(uint8_t data);

void dra_five(int xPos, int yPos);

void dra_two(int xPos, int yPos);

void dra_three(int xPos, int yPos);

void dra_six(int xPos, int yPos);

void dra_seven(int xPos, int yPos);

void dra_one(int xPos, int yPos);

void dra_zero(int xPos, int yPos);

void dra_four(int xPos, int yPos);

void dra_nine(int xPos, int yPos);

void dra_eight(int xPos, int yPos);

void draw_Numm(int num, int xPos,int yPos);

void draw_X(int xPos, int yPos);

void draw_Y(int xPos, int yPos);

void draw_Z(int xPos, int yPos);

void draw_Point(int xPos, int yPos);

void dra_blank(int xPos, int yPos);

void dra_OneDigitBlank(int xPos, int yPos);

void dra_Negative(int xPos, int yPos);

void erase_Negative(int xPos, int yPos);

void draw_Title(void);