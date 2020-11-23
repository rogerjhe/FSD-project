/**
 *  @file   GUI.cpp
 *  @brief  GUI main
 *  @author Team Avengers
 *  @date   2020-11-16
 ***********************************************/

// Include some standard libraries
// The 'C-based' libraries are needed for Nuklear GUI
#include <string.h>

// Include our Third-Party SFML header
// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

// NUKLEAR - for our GUI
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SFML_GL2_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sfml_gl2.h"

// Include standard library C++ libraries.
#include <iostream>
#include "GUI.hpp"
// Project header files
#include "App.hpp"
#include "Command.hpp"
#include "Draw.hpp"
#include "ClearCanvas.hpp"
#include <memory>

// Global commbo box color value
// For some reason it won't work without a global storing it.
// If you store it as an attribute of GUI, it will crash the program.
// Not worth the headache of enccapulating it.
static struct nk_colorf combo_box_color = {0.25f, 0.75f, 0.25f, 1.0f};

GUI::GUI(App* a) {
    // Canvas to draw GUI on
    app = a;

    // Pixel size of brush
    brush_size = 1;

    // Internet connection
    connection = false;
    // Use preset colors or color picker
    preset = true;
    // Index of current color
    preset_index = 1;
    // List to store preset color options
    this->setPresetColors();
    struct nk_colorf bg_pointer = {0.0f, 0.0f, 0.0f, 1.0f}; 
    bg = &bg_pointer;
    // Initializes GUI window
    this->Init();
}

GUI::~GUI() {
}

void GUI::setPresetColors(){
    preset_colors[0] = "";
    preset_colors[1] = "Black";
    preset_colors[2] = "White";
    preset_colors[3] = "Red";
    preset_colors[4] = "Green";
    preset_colors[5] = "Blue";
    preset_colors[6] = "Yellow";
    preset_colors[7] = "Magenta";
    preset_colors[8] = "Cyan";
}

int GUI::getPreset() {
    return preset_index;
}

void GUI::Init() {
    // Setting up context
    sf::ContextSettings settings(24, 8, 4, 2, 2);
    // Create a rendering window where we can draw an image on
    // Height is extended so GUI stays on screen if window is resized
    window = new sf::RenderWindow(sf::VideoMode(250, app->GetWindowHeight()*2), "GUI Window",sf::Style::Default,settings);
    window->setVerticalSyncEnabled(true);
    window->setActive(true);
    glViewport(0, 0, window->getSize().x, window->getSize().y);
    ctx = nk_sfml_init(window);
    struct nk_font_atlas *atlas;
    nk_sfml_font_stash_begin(&atlas);
    nk_sfml_font_stash_end();
    if (window->isOpen()) {
        std::cout << "Gui window works" << std::endl;
    }
}

void GUI::loop() {
    sf::Event event;
    // Load Fonts: if none of these are loaded a default font will be used
    //Load Cursor: if you uncomment cursor loading please hide the cursor
    nk_input_begin(ctx);
    while(window->pollEvent(event)){
        // Capture any keys that are released
        if(event.type == sf::Event::KeyReleased){
                //std::cout << "Key Pressed" << std::endl;
                // Check if the escape key is pressed.
            if(event.key.code == sf::Keyboard::Escape){
                nk_sfml_shutdown();
                window->close();
                exit(EXIT_SUCCESS);
            }
        }
        nk_sfml_handle_event(&event);
    }
    // Complete input from nuklear GUI
    nk_input_end(ctx);

    // Draw our GUI
    this->drawLayout();
    
    // OpenGL is the background rendering engine,
    // so we are going to clear our GUI graphics system.
    window->setActive(true);
    window->clear();
    glClearColor(bg->r, bg->g, bg->b, bg->a);
    glClear(GL_COLOR_BUFFER_BIT);
    nk_sfml_render(NK_ANTI_ALIASING_ON);
    window->display();

}
struct nk_context* GUI::getContext() {
    return ctx;
}

sf::RenderWindow* GUI::getWindow() {
    return window;
}

sf::Color GUI::GetInputColor(){
    sf::Color color;
    switch (preset_index)
    {
    case 1:
        color = sf::Color::Black;
        break;
    case 2:
        color = sf::Color::White;
        break;
    case 3:
        color = sf::Color::Red;
        break;
    case 4:
        color = sf::Color::Green;
        break;
    case 5:
        color = sf::Color::Blue;
        break;
    case 6:
        color = sf::Color::Yellow;
        break;
    case 7:
        color = sf::Color::Magenta;
        break;
    case 8:
        color = sf::Color::Cyan;
        break;        
    default:
        break;
    }
    return color;
}


/*! \brief Used in the case of user using shortcut keys to change color
    Changes the preset index and preset boolean so GUI can update.
    \param key_pressed int of the key pressed
*/
void GUI::UsePreset(int key_pressed) {
    preset_index = key_pressed;
    preset = true;
}

void GUI::setPresetIndex(int i) {
    if (i < 9 && i >= 0) {
        this->preset_index = i;
    }
}
 
 /*! \brief Creates the lay out of the GUI. Read the individual elements for more information
    \param ctx context needed to initialize nuklear
    \param nk_colorf color of the background
*/
void GUI::drawLayout(){
    /* GUI sets to toolbar to be flushed with top left corner
        Extends 250 pixels to the right and the window height
        Gives the window a border and title
        Removed features like minimize and ability to move it around
    */

    /* GUI window inside of canvas*/
    if (nk_begin(ctx, "Toolbar", nk_rect(0, 0, 250, app->GetWindowHeight()*3), NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
        // Mock design for filters
        nk_layout_row_begin(ctx, NK_STATIC, 30, 1);
        {
            nk_layout_row_push(ctx, 100);
            nk_label(ctx, "Filters:", NK_TEXT_LEFT);
        }
        nk_layout_row_end(ctx);
        nk_layout_row_static(ctx, 25, 100, 2);
        if (nk_button_label(ctx, "Sepia")){
             fprintf(stdout, "button pressed\n");
        }
        if (nk_button_label(ctx, "Gray Scale")){
            fprintf(stdout, "button pressed\n");
        }
        if (nk_button_label(ctx, "Sharpen")){
            fprintf(stdout, "button pressed\n");
        }
        if (nk_button_label(ctx, "Blur")){
            fprintf(stdout, "button pressed\n");
        }
        nk_spacing(ctx, 1);


        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "Preset Colors:", NK_TEXT_LEFT);
        preset_index = nk_combo(ctx, preset_colors, NK_LEN(preset_colors), preset_index, 25, nk_vec2(200,200));
        // Index of the combo box and will be 0 if color picker is in use
        if (preset_index > 0) {
            preset = true;
            app->ChangeColor(this->GetInputColor());
        }


        // Color picker to choose a more exact color
        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "Color Picker:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 20, 1);
        if (nk_combo_begin_color(ctx, nk_rgb_cf(combo_box_color), nk_vec2(nk_widget_width(ctx),400))) {
            nk_layout_row_dynamic(ctx, 120, 1);
            combo_box_color = nk_color_picker(ctx, combo_box_color, NK_RGBA);
            nk_layout_row_dynamic(ctx, 25, 1);
            combo_box_color.r = nk_propertyf(ctx, "#R:", 0, combo_box_color.r, 1.0f, 0.01f,0.005f);
            combo_box_color.g = nk_propertyf(ctx, "#G:", 0, combo_box_color.g, 1.0f, 0.01f,0.005f);
            combo_box_color.b = nk_propertyf(ctx, "#B:", 0, combo_box_color.b, 1.0f, 0.01f,0.005f);
            combo_box_color.a = nk_propertyf(ctx, "#A:", 0, combo_box_color.a, 1.0f, 0.01f,0.005f);
            nk_combo_end(ctx);
            uint8_t r = combo_box_color.r*255;
            uint8_t g = combo_box_color.g*255;
            uint8_t b = combo_box_color.b*255;
            uint8_t a = combo_box_color.a*255;
            app->ChangeColor(sf::Color(r,g,b,a));
            preset = false;
            preset_index = 0;
        }

        // Buttons for required features
        nk_layout_row_static(ctx, 20, 200, 1);
        if (nk_button_label(ctx, "Fill Screen")){
            std::shared_ptr<Command> clear_command = std::make_shared<ClearCanvas>("clear",
                        app->GetCurrentColor(), app->GetBackgroundColor(), *app);
            app->AddCommand(clear_command);
            app->ExecuteCommand();
        }

        // Brush
        nk_spacing(ctx, 1);
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_label(ctx, "Brush Size:", NK_TEXT_LEFT);
        nk_property_int(ctx, "<-Slide->", 0, &brush_size, 50, 1, 1);

        // Undo and redo buttons
        nk_layout_row_static(ctx, 20, 100, 2);
        nk_spacing(ctx, 2);
        if (nk_button_label(ctx, "Undo")){
            app->Undo();
        }
        if (nk_button_label(ctx, "Redo")){
            app->Redo();
        }
        
        // Place holder for connection interface:
        nk_layout_row_static(ctx, 20, 200, 1);
        nk_spacing(ctx, 1);
        nk_label(ctx, "Connection:", NK_TEXT_LEFT);
        nk_layout_row_static(ctx, 50, 50, 1);
        if (connection == 0){
            if (nk_button_symbol(ctx, NK_SYMBOL_CIRCLE_OUTLINE)){
                fprintf(stdout, "Connected\n");
                connection = 1;
            }
        }
        else{
            if (nk_button_symbol(ctx, NK_SYMBOL_CIRCLE_SOLID)){
                fprintf(stdout, "Disconnect\n");
                connection = 0;
            }            
        }

    }
    nk_end(ctx);
}

