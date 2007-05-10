/*
    This file is part of Akregator.

    Copyright (C) 2004 Stanislav Karchebny <Stanislav.Karchebny@kdemail.net>
                  2004-2005 Frank Osterfeld <frank.osterfeld@kdemail.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

    As a special exception, permission is given to link this program
    with any edition of Qt, and distribute the resulting executable,
    without including the source code for Qt in the source distribution.
*/
#include "article.h"
#include "folder.h"
#include "fetchqueue.h"
#include "treenodevisitor.h"

#include <qdom.h>
#include <QStringList>
#include <QList>

#include <KIcon>
#include <kdebug.h>

namespace Akregator {

class Folder::FolderPrivate
{
    public:
        /** List of children */
        QList<TreeNode*> children;
        /** caching unread count of children */
        int unread;
        /** whether or not the folder is expanded */
        bool open;

        /** caches guids for notifying added articles */
        QList<Article> addedArticlesNotify;
        /** caches guids for notifying removed articles */
        QList<Article> removedArticlesNotify;
};
           
bool Folder::accept(TreeNodeVisitor* visitor)
{
    if (visitor->visitFolder(this))
        return true;
    else
        return visitor->visitTreeNode(this);
}

Folder* Folder::fromOPML(const QDomElement& e)
{
    Folder* fg = new Folder(e.hasAttribute(QString::fromLatin1("text")) ? e.attribute(QString::fromLatin1("text")) : e.attribute(QString::fromLatin1("title")));
    fg->setOpen( e.attribute(QString::fromLatin1("isOpen")) == QString::fromLatin1(("true")));
    fg->setId( e.attribute(QString::fromLatin1("id")).toUInt() );
    return fg;
}

Folder::Folder(const QString& title) : TreeNode(), d(new FolderPrivate)
{
    d->unread = 0;
    setTitle(title);
} 

Folder::~Folder()
{
    TreeNode* tmp = 0;
    for (QList<TreeNode*>::ConstIterator it = d->children.begin(); it != d->children.end(); ++it)
    {
        delete tmp;
        tmp = *it;
    }
    delete tmp;
    
    emitSignalDestroyed();

    delete d;
    d = 0;
}

QList<Article> Folder::articles()
{
    QList<Article> seq;
    QList<TreeNode*>::ConstIterator en = d->children.end();
    for (QList<TreeNode*>::ConstIterator it = d->children.begin(); it != en; ++it)
        seq += (*it)->articles();
     
    return seq;
}

QDomElement Folder::toOPML( QDomElement parent, QDomDocument document ) const
{
    QDomElement el = document.createElement( "outline" );
    el.setAttribute( "text", title() );
    parent.appendChild( el );
    el.setAttribute("isOpen", d->open ? "true" : "false");
    el.setAttribute( "id", QString::number(id()) );

    QList<TreeNode*>::ConstIterator en = d->children.end();
    for (QList<TreeNode*>::ConstIterator it = d->children.begin(); it != en; ++it)
        el.appendChild( (*it)->toOPML(el, document) );
        
    return el;
}

QList<TreeNode*> Folder::children() const
{
    return d->children;
}

void Folder::insertChild(TreeNode* node, TreeNode* after)
{
    int pos = d->children.indexOf(after);
    
    if (pos < 0)
        prependChild(node);
    else 
        insertChild(pos+1, node);
}

QIcon Folder::icon() const
{
    return KIcon("folder");
}

void Folder::insertChild(int index, TreeNode* node)
{
//    kDebug() << "enter Folder::insertChild(int, node) " << node->title() << endl;
    if (node)
    {
        if (index >= d->children.size())
            d->children.append(node);
        else
            d->children.insert(index, node);
        node->setParent(this);
        connectToNode(node);
        updateUnreadCount();
        emit signalChildAdded(node);
        d->addedArticlesNotify += node->articles();
        articlesModified();
        nodeModified(); 
    }   
//    kDebug() << "leave Folder::insertChild(int, node) " << node->title() << endl; 
}

void Folder::appendChild(TreeNode* node)
{
//    kDebug() << "enter Folder::appendChild() " << node->title() << endl;
    if (node)
    {
        d->children.append(node);
        node->setParent(this);
        connectToNode(node);
        updateUnreadCount();
        emit signalChildAdded(node);
        d->addedArticlesNotify += node->articles();
        articlesModified();
        nodeModified();
    }    
//    kDebug() << "leave Folder::appendChild() " << node->title() << endl;
}

void Folder::prependChild(TreeNode* node)
{
//    kDebug() << "enter Folder::prependChild() " << node->title() << endl;
    if (node)
    {
        d->children.prepend(node);
        node->setParent(this);
        connectToNode(node);
        updateUnreadCount();
        emit signalChildAdded(node);
        d->addedArticlesNotify += node->articles();
        articlesModified();
        nodeModified();
    }    
//    kDebug() << "leave Folder::prependChild() " << node->title() << endl;
}

void Folder::removeChild(TreeNode* node)
{
//    kDebug() << "enter Folder::removeChild() node:" << (node ? node->title() : "null") << endl;
    if (node && d->children.contains(node))
    {    
        node->setParent(0);
        d->children.removeAll(node);
        disconnectFromNode(node);
        updateUnreadCount();    
        emit signalChildRemoved(this, node);
        d->removedArticlesNotify += node->articles();
        articlesModified(); // articles were removed, TODO: add guids to a list
        nodeModified();
    }
//    kDebug() << "leave Folder::removeChild() node: " << (node ? node->title() : "null") << endl;
}


TreeNode* Folder::firstChild()
{
    return d->children.isEmpty() ? 0 : d->children.first();
}            

TreeNode* Folder::lastChild()
{
    return d->children.isEmpty() ? 0 : d->children.last();
}
            
bool Folder::isOpen() const
{
    return d->open;
}

void Folder::setOpen(bool open)
{
    d->open = open;
}
            
int Folder::unread() const
{
    return d->unread;
}

int Folder::totalCount() const
{
    int totalCount = 0;

    QList<TreeNode*>::ConstIterator en = d->children.end();
    for (QList<TreeNode*>::ConstIterator it = d->children.begin(); it != en; ++it)
        totalCount += (*it)->totalCount();
    
    return totalCount;
}

void Folder::updateUnreadCount()
{
    int unread = 0;

    QList<TreeNode*>::ConstIterator en = d->children.end();
    for (QList<TreeNode*>::ConstIterator it = d->children.begin(); it != en; ++it)
        unread += (*it)->unread();
    
    d->unread = unread;
}

void Folder::slotMarkAllArticlesAsRead() 
{
    setNotificationMode(false);
    QList<TreeNode*>::ConstIterator en = d->children.end();
    for (QList<TreeNode*>::ConstIterator it = d->children.begin(); it != en; ++it)
        (*it)->slotMarkAllArticlesAsRead();
    setNotificationMode(true, true);
}
    
void Folder::slotChildChanged(TreeNode* /*node*/)
{
    updateUnreadCount();
    nodeModified();
}

void Folder::slotChildDestroyed(TreeNode* node)
{
    d->children.removeAll(node);
    updateUnreadCount();    
    nodeModified();
}

void Folder::slotDeleteExpiredArticles()
{
    setNotificationMode(false);
    QList<TreeNode*>::ConstIterator en = d->children.end();
    for (QList<TreeNode*>::ConstIterator it = d->children.begin(); it != en; ++it)
        (*it)->slotDeleteExpiredArticles();
    setNotificationMode(true, true);
}

void Folder::slotAddToFetchQueue(FetchQueue* queue, bool intervalFetchOnly)
{
    QList<TreeNode*>::ConstIterator en = d->children.end();
    for (QList<TreeNode*>::ConstIterator it = d->children.begin(); it != en; ++it)
        (*it)->slotAddToFetchQueue(queue, intervalFetchOnly);
}

void Folder::doArticleNotification()
{
}

void Folder::connectToNode(TreeNode* child)
{
        connect(child, SIGNAL(signalChanged(Akregator::TreeNode*)), this, SLOT(slotChildChanged(Akregator::TreeNode*)));
        connect(child, SIGNAL(signalDestroyed(Akregator::TreeNode*)), this, SLOT(slotChildDestroyed(Akregator::TreeNode*)));
        connect(child, SIGNAL(signalArticlesAdded(Akregator::TreeNode*, QList<Akregator::Article>)), this, SIGNAL(signalArticlesAdded(Akregator::TreeNode*, QList<Akregator::Article>)));
        connect(child, SIGNAL(signalArticlesRemoved(Akregator::TreeNode*, QList<Akregator::Article>)), this, SIGNAL(signalArticlesRemoved(Akregator::TreeNode*, QList<Akregator::Article>)));
        connect(child, SIGNAL(signalArticlesUpdated(Akregator::TreeNode*, QList<Akregator::Article>)), this, SIGNAL(signalArticlesUpdated(Akregator::TreeNode*, QList<Akregator::Article>)));
}

void Folder::disconnectFromNode(TreeNode* child)
{
        disconnect(child, SIGNAL(signalChanged(Akregator::TreeNode*)), this, SLOT(slotChildChanged(Akregator::TreeNode*)));
        disconnect(child, SIGNAL(signalDestroyed(Akregator::TreeNode*)), this, SLOT(slotChildDestroyed(Akregator::TreeNode*)));
        disconnect(child, SIGNAL(signalArticlesAdded(Akregator::TreeNode*, QList<Akregator::Article>)), 
                   this, SIGNAL(signalArticlesAdded(Akregator::TreeNode*, QList<Akregator::Article>)));
        disconnect(child, SIGNAL(signalArticlesRemoved(Akregator::TreeNode*, QList<Akregator::Article>)),
                   this, SIGNAL(signalArticlesRemoved(Akregator::TreeNode*, QList<Akregator::Article>)));
        disconnect(child, SIGNAL(signalArticlesUpdated(Akregator::TreeNode*, QList<Akregator::Article>)), 
                   this, SIGNAL(signalArticlesUpdated(Akregator::TreeNode*, QList<Akregator::Article>)));
}
            
TreeNode* Folder::next()
{
    if ( firstChild() )
        return firstChild();

    if ( nextSibling() )
        return nextSibling();
    
    Folder* p = parent();
    while (p)
    {
        if ( p->nextSibling() )
            return p->nextSibling();
        else
            p = p->parent();
    }
    return 0;
}

} // namespace Akregator
#include "folder.moc"
