





class Game {
public:
    Game();
    ~Game();

    void Run();
    void Initialize();
    void Shutdown();

    class Renderer* GetRenderer() { return renderer; }

private:

    void ProcessInput();
    void Update();
    void GenerateOutput();

    unsigned int ticksCount;

    bool isRunning;

    class Renderer* renderer; // Forward declaration of Renderer class

};
