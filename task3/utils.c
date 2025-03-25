#include <stdint.h>

#define HEIGHT 25
#define WIDTH 80
#define VGA_MEMORY 0xb8000
#define VGA_ATTRIBUTE 0x0F

#define SPACE_SYMBOL 0x0020


int abs(int v) {
    return (v >= 0) ? v : -v;
}


typedef struct {
    int x;
    int y;
} cur_t;


cur_t cur;


void init_cur() {
    cur.x = 0;
    cur.y = 0;
}

// очищает экран
void clear() {
    int16_t* vga_buffer = (int16_t*)VGA_MEMORY;
    for (unsigned int i = 0; i < HEIGHT * WIDTH; ++i)
        *(vga_buffer + i * sizeof(int16_t)) = 0x0020;
    cur.x = 0;
    cur.y = 0;
}

// выводит символ c в строке x и столбце y
// ожидается, что x - в диапазоне [0, 24]
// а y - в диапазоне - [0, 79]
// позиция (0, 0) - в левом верхнем углу
void putc_pos(int x, int y, char c) {
    int16_t* vga_buffer = (int16_t*)VGA_MEMORY;
    vga_buffer[x * WIDTH + y] = (VGA_ATTRIBUTE << 8) | c;
}


void _shift_lines() {
    for (unsigned int row = 1; row < HEIGHT; ++row)
        for (unsigned int col = 0; col < WIDTH; ++col)
            putc_pos(row - 1, col, ((int16_t*)VGA_MEMORY)[row * WIDTH + col]);
    for (unsigned int col = 0; col < WIDTH; ++col)
        putc_pos(HEIGHT - 1, col, SPACE_SYMBOL);
}

// поддерживает понятие текущей позиции 
// после вызова clear текущая позиция - 0, 0 
// Обычный символ выводится в текущую позицию 
// после чего y-координата увеличивается на 1,
// а если она превращается в 80, то ее обнуляем
// и увеличиваем x на 1
// если перед увеличением на 1 x уже равнялось 24,
// то сдвигаем все строки на 1 вверх, очищаем сасую нижнюю
// и текущей позицией делаем (24, 0)
//
// Вывоз puts('\n') должен приводить к "переводу строки"
// (если текущая позиция была в самой нижней строке, то
// сдвигаем строки на 1 вверх, очищаем сасую нижнюю
// и текущей позицией делаем (24, 0)



void putc(char c) {
    if (c == '\n') {
        cur.y = 0;
        cur.x++;
    } else {
        putc_pos(cur.x, cur.y, c);        
        cur.y++;
        if (cur.y == 80) {
            cur.y = 0;
            cur.x++;
        }
    }

    if (cur.x == 25) {
        _shift_lines();
        cur.x = 24;
    }
}

// выводит строку s, начиная со строки x и столбце y
// ожидается, что x - в диапазоне [0, 24]
// а y - в диапазоне - [0, 79]
// позиция (0, 0) - в левом верхнем углу
// Если достигли конца строки - переходим на следующую
// Если достигли конца экрана - сдвигаем строки 
void puts_pos(int x, int y, const char *s) {
    while (*s) {
        putc_pos(x, y, *s);
        s++;
        y++;
        if (y == 80) {
            y = 0;
            x++;
        }
        if (x == 25) {
            _shift_lines();
            x = 24;
        }
    }
}

// выводит строку s, начиная с текущей позиции
// обновляя ее
// (по сути - вызываем putc в цикле для каждого символа)
void puts(const char *s) {
    while (*s) {
        putc(*s);
        s++;
    }
}


void itos(int v, char* s) {
    int sign = (v >= 0) ? 1 : -1;
    v = abs(v);


    const int mx_dig_cnt = 11;


    char* digits[mx_dig_cnt];
    for (unsigned int i = 0; i < mx_dig_cnt; ++i) {
        digits[i] = (v % 10) + '0';
        v /= 10;
    }
        

    int gdigit = mx_dig_cnt;
    while (gdigit > 0) {
        gdigit--;
        if (digits[gdigit] != 0 + '0')
            break;
    }

    if (sign == -1)
        digits[++gdigit] = '-';
    
    
    for (unsigned int i = 0; i <= gdigit; ++i)
        s[i] = digits[gdigit - i];
    s[gdigit + 1] = '\0';
}

// выводит десятичное число со знаком и без лишних ведущих нулей
// только значимые цифры
// Начиная с заданной позиции


void putnum_pos(int x, int y, int v) {
    char digits[10];
    itos(v, digits);
    puts_pos(x, y, digits);
}

// выводит десятичное число со знаком и без лишних ведущих нулей
// только значимые цифры
// Начиная с текущей позиции и обновляя ее. Без перевода строки в конце.
void putnum(int v) {
    char digits[20];
    itos(v, digits);
    puts(digits);
}


void itohexs(unsigned int v, unsigned int n, char* s) {
    s[0] = '0';
    s[1] = 'x';

    const char hdigits[] = "0123456789ABCDEF";

    for (int i = n - 1; i >= 0; i--) {
        uint8_t digit = ((long long)v >> (4 * i)) & 0xF;
        s[n - i + 1] = hdigits[digit]; 
    }
    s[n + 2] = '\0';
}

// выводит 0x и заданное число младших 16-ричных цифр 
// Начиная с заданной позиции
// Ожидается, что n - в диапазоне [0, 16]
// Пример: puthex(0, 0, 0x12345, 3) должно вывести 0x345 в левом верхнем углу экрана
void puthex_pos(int x, int y, unsigned int v, unsigned int n) {
    char s[19];
    itohexs(v, n, s);
    puts_pos(x, y, s);
}

// выводит 0x и заданное число младших 16-ричных цифр
// Начиная с текущей позиции и обновляя ее
// Ожидается, что n - в диапазоне [0, 16]
// Пример: puthex(0, 0, 0x12345, 3) должно вывести 0x345 в левом верхнем углу экрана
void puthex(unsigned int v, unsigned int n) {
    char s[19];
    itohexs(v, n, s);
    puts(s);
}


void init() {
    init_cur();
}





struct idtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed));


void print_idtr() {
    struct idtr idtr;
    __asm__ volatile("sidt %0" : "=m"(idtr));

    char s[20];
    itohexs(idtr.limit, 4, s);
    puts(s);
    putc(' ');
    itohexs(idtr.base, 8, s);
    puts(s);
    putc('\n');

    for (int i = 15; i >= 0; i--) {
        putc(((idtr.limit >> i) & 1) + '0');
        if (i % 4 == 0)
            putc(' ');
    }
    for (int i = 31; i >= 0; i--) {
        putc(((idtr.base >> i) & 1) + '0');
        if (i % 4 == 0)
            putc(' ');
    }
    putc('\n');

    char t[100];
    puts("limit: ");
    itos(idtr.limit, t);
    puts(t);
    putc(' ');
    puts("base: ");
    itos(idtr.base, t);
    puts(t);
    putc('\n');
}


void print_idt(unsigned int start, unsigned int n) {
    struct idtr idtr;
    __asm__ volatile("sidt %0" : "=m"(idtr));

    if ((start + n) * 8 > idtr.limit + 1) {
        puts("error: there are no such lines in idt\n");
        return;
    }

    for (unsigned int i = start; i < start + n; i++) {
        struct idt_entry* entry = (struct idt_entry*)(idtr.base + i * 8);

        char buffer[20];
        itohexs(entry->offset_low, 4, buffer);
        puts(buffer);
        putc(' ');

        itohexs(entry->selector, 4, buffer);
        puts(buffer);
        putc(' ');

        itohexs(entry->zero, 2, buffer);
        puts(buffer);
        putc(' ');

        itohexs(entry->type_attr, 2, buffer);
        puts(buffer);
        putc(' ');

        itohexs(entry->offset_high, 4, buffer);
        puts(buffer);
        putc('\n');

        for (int j = 15; j >= 0; j--) {
            putc(((entry->offset_low >> j) & 1) + '0');
            if (j % 4 == 0)
                putc(' ');
        }
        for (int j = 15; j >= 0; j--) {
            putc(((entry->selector >> j) & 1) + '0');
            if (j % 4 == 0)
                putc(' ');
        }
        for (int j = 7; j >= 0; j--) {
            putc(((entry->zero >> j) & 1) + '0');
            if (j % 4 == 0)
                putc(' ');
        }
        for (int j = 7; j >= 0; j--) {
            putc(((entry->type_attr >> j) & 1) + '0');
            if (j % 4 == 0)
                putc(' ');
        }
        for (int j = 15; j >= 0; j--) {
            putc(((entry->offset_high >> j) & 1) + '0');
            if (j % 4 == 0)
                putc(' ');
        }
        putc('\n');

        char dec_buffer[20];
        
        puts("offset low: ");
        itos(entry->offset_low, dec_buffer);
        puts(dec_buffer);
        putc('\n');
        
        puts("selector: ");
        itos(entry->selector, dec_buffer);
        puts(dec_buffer);
        putc('\n');
        
        puts("zero: ");
        itos(entry->zero, dec_buffer);
        puts(dec_buffer);
        putc('\n');
        
        puts("type attr: ");
        itos(entry->type_attr, dec_buffer);
        puts(dec_buffer);
        putc('\n');
        
        puts("offset high: ");
        itos(entry->offset_high, dec_buffer);
        puts(dec_buffer);
        putc('\n');
        putc('\n');
    }
}

int main() {
    print_idtr();
    print_idt(6, 3);
    while (1) {}
}
