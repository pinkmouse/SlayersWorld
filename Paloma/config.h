#ifndef CONFIG_H
#define CONFIG_H


class Config
{
public:
    Config();
    void SetTileLevel(int);
    int GetTileLevel() const;

private:
    int m_TileLevel;
};

#endif // CONFIG_H
