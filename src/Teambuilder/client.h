#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui>
#include "analyze.h"
#include "../PokemonInfo/networkstructs.h"

class MainWindow;

class Player
{
public:
    int id;
    BasicInfo team;
};

class ChallengeWindow;
class QIdListWidgetItem;

QDataStream & operator >> (QDataStream &in, Player &p);
QDataStream & operator << (QDataStream &out, const Player &p);

/* The class for going online */

class Client : public QWidget
{
    Q_OBJECT
public:
    Client(TrainerTeam *, const QString &url);

    TrainerTeam *team();
    QMenuBar *createMenuBar(MainWindow *w);

    void printLine(const QString &line);
    bool playerExist(int id) const;
    Player player(int id) const;
    BasicInfo info(int id) const;
public slots:
    void errorFromNetwork(int errnum, const QString &error);
    void connected();
    void disconnected();
    /* message received from the server */
    void messageReceived(const QString & mess);
    /* sends what's in the line edit */
    void sendText();
    void playerLogin(const Player &p);
    void playerReceived(const Player &p);
    void playerLogout(int);
    /* sends the server a challenge notice */
    void sendChallenge(int id);
    /* removes the pointer to the challenge window when it is destroyed */
    void clearChallenge();
    void seeInfo(int id);
    /* shows the context menu for that player */
    void showContextMenu(const QPoint&);
signals:
    void done();

private:
    TrainerTeam *myteam;
    /* Main chat */
    QTextEdit *mychat;
    /* Line the user types in */
    QLineEdit *myline;
    /* Where players are displayed */
    QListWidget *myplayers;
    QMap<int, QIdListWidgetItem *> myplayersitems;
    /* Button to exit */
    QPushButton *myexit;
    /* Button to send text */
    QPushButton *mysender;
    /* Network Relay */
    Analyzer myrelay;
    /* Challenge window */
    ChallengeWindow *mychallenge;
    /* Utility */
    QSignalMapper mymapper;

    QMap<int, BasicInfo> myplayersinfo;
    QTextEdit *mainChat();
    Analyzer & relay();

    void initRelay();
};

#endif // CLIENT_H