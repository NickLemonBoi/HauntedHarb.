#include "Game.h"

Game::Game() {
    start_level("Level1.txt");
}

void Game::inputs(WPARAM key, bool up) {
    switch (key) {
    case 0x41: //A
        Vic->xspeed = up ? 0 : -PSPEED;
        break;
    case 0x44: //D
        Vic->xspeed = up ? 0 : PSPEED;
        break;
    case 0x53: //S
        //WILL IMPLEMENT LATER
        break;
    case 0x57: //W
        if (Vic->grounded || Vic->landed) Vic->yspeed = -PSPEED * 2;
        break;
    case VK_SPACE:
        Vic->shooting = !up;
            break;
   }
}

void Game::update() {
    if (php < 1) reset_game();
    Vic->update(boxes, platforms);
    for (Platform* p : platforms) p->update();
    for (Enemy* e : enemies) e->update(Vic, boxes, platforms);
    for (Spawner* s : spawners) s->update(Vic, boxes, platforms);
}

void Game::render(HDC buffer_context) {
    bg->draw(buffer_context);
    ground->draw(buffer_context);
    for (Box* b : boxes) b->draw(buffer_context);
    for (Platform* p : platforms) p->draw_boxes(buffer_context);
    for (Enemy* e : enemies) e->draw(buffer_context);
    for (Enemy* e : enemies) e->draw_bullets(buffer_context);
    for (Spawner* s : spawners) s->draw_crabs(buffer_context);
    Vic->draw_bullets(buffer_context);
    Vic->draw(buffer_context);
    health->draw(buffer_context);
}

void Game::reset_game() {
    boxes.clear();
    platforms.clear();
    enemies.clear();
    spawners.clear();
    start_level("Level1.txt");
}

//level loading
list<list<string>> Game::load_level(CString lvl) {
    std::ifstream file("./level/" + lvl);
    string str;
    list<string> lines;
    list<list<string>> level;

    while (std::getline(file, str)) lines.add(str);

    string de = "|"; //setting delimiter to pipe character NOT L or I
    list<string> elements;
    size_t p = 0;
    for (string s : lines) {
        while ((p = s.find(de)) != string::npos) {
            elements.add(s.substr(0, p));
            s.erase(0, p + de.length());
        }
        level.add(elements);
        elements.clear();
    }
    return level;


}

void Game :: build_level(list<list<string>> lvl) {
    int r = 0, c = 0; //Width, Height, row in lvl, column in lvl
    for (list<string> line : lvl) {
        for (string s : line) {

            if (s == "[C]") boxes.add(new Box("SmallCrate.bmp", c * gcellw, r * gcellh, gcellw, gcellh));
            if (s == "[I]") boxes.add(new Box("iceblock.png", c * gcellw, r * gcellh, gcellw, gcellh));
            if (s == "[L]") boxes.add(new Box("Lava.png", c * gcellw, r * gcellh, gcellw, gcellh));
            if (s == "[T]") boxes.add(new Box("toxicblock.png", c * gcellw, r * gcellh, gcellw, gcellh));
            if (s == "[W]") boxes.add(new Box("woodblock.png", c * gcellw, r * gcellh, gcellw, gcellh));
            if (s == "[B]") boxes.add(new Box("Crate.bmp", c * gcellw, r * gcellh, gcellw * 2, gcellh * 2));
            if (s == "[S]") spawners.add(new Spawner(c * gcellw, r * gcellh, 3, 65 * 4));
            if (s == "[K]") enemies.add(new Crab(c * gcellw, r * gcellh));
            if (s == "[P]") {
                Vic = new Player(c * gcellw, r * gcellh);
                mpos = Vic ->x - 100;
            }

            /*END*/ if (c++ == line.size() - 1) c = 0;
        }
        /*END ROWS*/ if (r++ == lvl.size() - 1) r = 0;
    }
} 

void Game :: start_level(CString lvl) {

    build_level(load_level(lvl));
}


