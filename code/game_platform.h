#if !defined(GAME_PLATFORM_H)

#include <stdint.h>
#include <stdbool.h>

//typedef float __attribute__((ext_vector_type(2))) v2;
typedef size_t size;
typedef int8_t i8;
typedef int16_t i16; 
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

typedef float f32;
typedef double f64;


#define assert(expresion) if(!(expresion)) __debugbreak();
#define array_count(value) (sizeof(value)/sizeof(value[0]))
#define array_length(value) (sizeof(value)/sizeof(value[0]))
#define array_size(value) (sizeof(value)/sizeof(value[0]))
#define ABS(n) ((n)<0 ? -(n) : (n))
#define PI 3.14159265f
#define RAD2DEG(n) ((180.0f/PI) * (n))
#define DEG2RAD(n) ((PI/180.0f) * (n))
#define RAD 0.0174533f

#define Kilobytes(Value) ((Value) * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

#define local_static static
#define global static


typedef enum EventMouse{MOUSE_NONE, MOUSE_LBUTTON, MOUSE_RBUTTON, MOUSE_MBUTTON, MOUSE_XBUTTON1, MOUSE_XBUTTON2, MOUSE_WHEEL} EventMouse;
typedef enum EventPad{PAD_NONE, PAD_UP, PAD_DOWN, PAD_LEFT, PAD_RIGHT, PAD_BACK} EventPad;
typedef enum EventKey{KEY_NONE, KEY_W, KEY_A, KEY_S, KEY_D, KEY_L, KEY_P, KEY_ESCAPE, KEY_1, KEY_2, KEY_3} EventKey;
typedef enum EventType{EVENT_NONE, EVENT_KEYDOWN, EVENT_KEYUP, EVENT_MOUSEWHEEL, EVENT_MOUSEDOWN, EVENT_MOUSEUP, EVENT_MOUSEMOTION, EVENT_TEXT, EVENT_PADDOWN, EVENT_PADUP} EventType;


typedef struct Event{
    EventType type;
    EventKey key;
    EventPad pad;
    EventMouse mouse;
    i32 mouse_x;
    i32 mouse_y;
    i8 wheel_y;
    i8 wheel_x;
    // QUESTION: ask about ui16 vs WCHAR
    ui16 text;
} Event;

typedef struct Events{
    Event event[256];
    ui32 size;
    ui32 index;
} Events;

typedef struct RenderBuffer{
    void *memory;
    int memory_size;

    int bytes_per_pixel;
    int width;
    int height;
    int pitch;
} RenderBuffer;

typedef struct FileData{
    ui32 size;
    void* content;
} FileData;

#include "vectors.h"
typedef struct Controller{
    bool up;
    bool down;
    bool left;
    bool right;
    bool m1;
    bool m2;
    v2 mouse_pos;
} Controller;

typedef struct Clock{
    f32 dt;
} Clock;

static int
string_length(char* s){
    int count = 0;
    while(*s++){
        ++count;
    }

    return(count);
}

static char *
string_point_at_last(char s[], char t, int count){
    char *result = NULL;
    char *found[10];
    ui32 i = 0;

    while(*s++){
        if(*s == t){
            found[i] = s + 1;
            i++;
        }
    }
    result = found[i - count];

    return(result);
}

static void
cat_strings(char *left, char *right, char *dest){
    int left_size = string_length(left);
    int right_size = string_length(right);

    for(int i=0; i < left_size; ++i){
        *dest++ = *left++;
    }
    for(int i=0; i < right_size; ++i){
        *dest++ = *right++;
    }

    *dest++ = 0;
}

static char*
end_of_string(char *str){
    while(*str++){
    }
    return(str);
}

static void
get_root_dir(char *left, i64 length, char *full_path){
    int i=0;
    for(;i < length; ++i){
        *left++ = *full_path++;
    }
    *left++ = 0;
}

static void
copy_string(char *src, char *dst, size_t size){
    for(ui32 i=0; i < size; ++i){
        *dst++ = *src++;
    }
}

static f32
f32_max(f32 a, f32 b){
    if(a >= b){
        return(a);
    }
    return(b);
}

static f32
f32_min(f32 a, f32 b){
    if(a <= b){
        return(a);
    }
    return(b);
}

#define READ_ENTIRE_FILE(name) FileData name(char *filename)
typedef READ_ENTIRE_FILE(ReadEntireFile);

#define WRITE_ENTIRE_FILE(name) bool name(char *filename, void *memory, ui32 memory_size)
typedef WRITE_ENTIRE_FILE(WriteEntireFile);

#define FREE_FILE_MEMORY(name) void name(void *memory)
typedef FREE_FILE_MEMORY(FreeFileMemory);


typedef struct GameMemory{
	bool running;
    bool initialized;

    void *total_storage; // IMPORTANT: REQUIRED to be cleared to zero at startup
    void *permanent_storage; // IMPORTANT: REQUIRED to be cleared to zero at startup
    void *transient_storage; // IMPORTANT: REQUIRED to be cleared to zero at startup

    ui64 total_size;
    ui64 permanent_storage_size;
    ui64 transient_storage_size;

    char root_dir[256];
    char data_dir[256];

    ReadEntireFile *read_entire_file;
    WriteEntireFile *write_entire_file;
    FreeFileMemory *free_file_memory;
} GameMemory;

#define MAIN_GAME_LOOP(name) void name(GameMemory *memory, RenderBuffer *render_buffer, Events *events, Clock *clock)
typedef MAIN_GAME_LOOP(MainGameLoop);


#define GAME_PLATFORM_H
#endif
