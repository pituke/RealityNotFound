/**
 Client - server communication implementation inspired by http://thesmithfam.org/blog/2009/07/09/example-qt-chat-program/
 */

#ifndef __3DVisualServer_H__
#define __3DVisualServer_H__

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>

#include "osg/PositionAttitudeTransform"

#include "Layout/LayoutThread.h"
#include "Viewer/CoreGraph.h"
#include "Network/ExecutorFactory.h"

namespace Network {

class Server : public QTcpServer {

    Q_OBJECT

    public:
        Server(QObject *parent=0);

        static Server* getInstance();

        void sendGraph(QTcpSocket *client = NULL);
        void sendLayout(QTcpSocket *client = NULL);
        void sendMoveNodes();

        void stopServer();

        void setSelectedNodes(QLinkedList<osg::ref_ptr<Data::Node> > nodes) { selected_nodes = nodes; }

        void sendMyView(osg::Vec3d center, osg::Quat rotation, QTcpSocket * client = NULL);
        void sendMyView(QTcpSocket * client);
        void sendMyView();

        void sendUserList();
        void updateUserList();

        void removeAvatar(QTcpSocket * client);
        osg::PositionAttitudeTransform * generateAvatar();
        void spyUser(int id);
        bool isSpying() { return user_to_spy != NULL; }
        void unSpyUser();

        void centerUser(int id_user);
        void unCenterUser() { user_to_center = NULL; }
        QTcpSocket * getCenterUser() { return user_to_center; }
        bool isCenteringUser() { return user_to_center != NULL; }

        void setMyView(osg::Vec3d center, osg::Quat rotation);
        void lookAt(osg::Vec3d coord);

        //some getters
        float getGraphScale() { return graphScale; }
        QSet<QTcpSocket*> getClients() { return clients; }
        Layout::LayoutThread * getLayoutThread() { return thread; }
        int getUserId(QTcpSocket * Client) { return usersID[Client]; }
        osg::PositionAttitudeTransform * getAvatarTransform(QTcpSocket* client) { return avatars[client]; }
        int getUserCount() {return usersID.count(); }
        int getMaxUserId();
        Vwr::CoreGraph * getCoreGraph() { return coreGraph; }
        QTcpSocket * userToSpy() { return user_to_spy; }

        //some setters
        void appendMovingNode(osg::ref_ptr<Data::Node> node);
        void addUser(QTcpSocket * socket,QString name,int id) { users[socket] = name; usersID[socket] = id; }
        void addAvatar(QTcpSocket* socket,osg::PositionAttitudeTransform * avatarTransform) { avatars[socket] = avatarTransform; }
        void setLayoutThread(Layout::LayoutThread * layoutThread);
        void setCoreGraph(Vwr::CoreGraph * cg) { coreGraph = cg; }

    private slots:
        void readyRead();
        void disconnected();

    protected:
        void incomingConnection(int socketfd);

    private:
        static Server *instance;
        QObject * cw;

        QLinkedList<osg::ref_ptr<Data::Node> > selected_nodes;
        QLinkedList<osg::ref_ptr<Data::Node> > moving_nodes;

        QSet<QTcpSocket*> clients;
        QMap<QTcpSocket*,QString> users;
        QMap<QTcpSocket*,int> usersID;
        QMap<QTcpSocket*,osg::PositionAttitudeTransform *> avatars;
        Layout::LayoutThread * thread;
        Vwr::CoreGraph * coreGraph;
        ExecutorFactory *executorFactory;
        float graphScale;

        QTcpSocket * user_to_spy;
        QTcpSocket * user_to_center;

        osg::Vec3d original_center;
        osg::Quat original_rotation;

        QTcpSocket * getClientById(int id);

        quint16 blockSize;
    };
}

#endif
