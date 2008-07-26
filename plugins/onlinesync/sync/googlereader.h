#ifndef GOOGLEREADER_H
#define GOOGLEREADER_H

#include <KApplication>
#include <QHttp>
#include <QMutex>

#include "aggregator.h"
#include "subscriptionlist.h"

class KConfigGroup;

namespace feedsync
{

class GoogleReader: public Aggregator
{
    Q_OBJECT

 public:
    explicit GoogleReader(const KConfigGroup& configgroup, QObject* parent=0);
    ~GoogleReader();
    SubscriptionList getSubscriptionList() const;
    void load();
    void add(const SubscriptionList & list);
    void update(const SubscriptionList & list);
    void remove(const SubscriptionList & list);

 protected Q_SLOTS:
    void slotAuthenticationDone(bool error);
    void slotListDone(bool error);
    void slotTokenDone(bool error);
    void slotAddDone(bool error);
    void slotUpdateDone(bool error);
    void slotRemoveDone(bool error);

 private:
    QHttp *http;
    QString _sid;
    QString _token;
    QString getSID() const;
    QString getUser() const;
    QString getPassword() const;
    QString getToken() const;
    void setUser(const QString& user);
    void setPassword(const QString& password);
    void setToken(const QString& token);
    SubscriptionList _subscriptionList;
    QString _user;
    QString _password;
    int _cursor;
    SubscriptionList _cursorList;
    void genError(const QString& msg);

 signals:
    void loadDone();
    void addDone();
    void updateDone();
    void removeDone();
    void error(const QString& msg);
};

}

#endif