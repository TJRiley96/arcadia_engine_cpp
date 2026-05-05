
const char* LOGGING_DIRECTORY = "logs";

class Logging {
public:
    static void LogInfo(const char* message);
    static void LogWarning(const char* message);
    static void LogError(const char* message);
};