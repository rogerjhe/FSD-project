/**
 *  @file   GUI.h
 *  @brief  GUI class interface
 *  @author Team Avengers
 *  @date   2020-11-16
 ***********************************************/
#ifndef GUI_HPP
#define GUI_HPP
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

#include "App.hpp"


class GUI {
    public:
        GUI(App* app);
        ~GUI();
        // Function to render our GUI
        void                    drawLayout();
        sf::Color               GetInputColor();
        void                    UsePreset(int key_pressed);
        void                    Init();
        void                    loop();
        struct nk_context*      getContext();
        sf::RenderWindow*       getWindow();
        void                    setPresetIndex(int i);
        int                     getPreset();

    private:
        App*                app;
        int                 brush_size;
        bool                connection;
        bool                preset;
        int                 preset_index;
        static const int    num_preset_colors = 9;
        const char*         preset_colors[num_preset_colors];
        struct nk_context*  ctx;
        sf::RenderWindow*   window;
        struct nk_colorf*   bg;
        void                setPresetColors();
};

#endif