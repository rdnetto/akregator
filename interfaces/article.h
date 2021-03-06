/*
    This file is part of Akregator.

    Copyright (C) 2004 Stanislav Karchebny <Stanislav.Karchebny@kdemail.net>
                  2005 Frank Osterfeld <osterfeld@kde.org>

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

#ifndef AKREGATOR_ARTICLE_H
#define AKREGATOR_ARTICLE_H

#include "akregatorinterfaces_export.h"
#include "types.h"

#include <Syndication/Person>
#include <QSharedPointer>

class QDateTime;
class QString;

template<class T> class QList;

typedef unsigned int uint;

class QUrl;

namespace Syndication {
class Enclosure;
class Item;
typedef QSharedPointer<Item> ItemPtr;
}

namespace Akregator {
namespace Backend {
class FeedStorage;
}

class Feed;
/** A proxy class for Syndication::ItemPtr with some additional methods to assist sorting. */
class AKREGATORINTERFACES_EXPORT Article
{
    friend class ArticleDeleteJob;
    friend class ArticleModifyJob;
    friend class Feed;

public:
    enum ContentOption {
        ContentAndOnlyContent, /*< returns the content field even if empty */
        DescriptionAsFallback /*< uses the description field as fallback if the content field is empty */
    };

    Article();
    /** creates am article object for an existing article.
        The constructor accesses the archive to load it's data
        */
    Article(const QString &guid, Feed *feed);
    /** creates an article object from a parsed librss Article
        the article is added to the archive if not yet stored, or updated if stored but modified
    */
    Article(const Syndication::ItemPtr &article, Feed *feed);

    Article(const Syndication::ItemPtr &article, Backend::FeedStorage *archive);
    Article(const Article &other);
    ~Article();

    void swap(Article &other)
    {
        std::swap(d, other.d);
    }

    Article &operator=(const Article &other);
    bool operator==(const Article &other) const;
    bool operator!=(const Article &other) const;

    bool isNull() const;

    int status() const;

    QString title() const;
    QUrl link() const;
    QString description() const;

    QString content(ContentOption opt = ContentAndOnlyContent) const;

    QString guid() const;
    /** if true, the article should be kept even when expired **/
    bool keep() const;

    bool isDeleted() const;

    void offsetPubDate(int secs);

    Feed *feed() const;

    /** returns a hash value used to detect changes in articles with non-hash GUIDs. If the guid is a hash itself, it returns @c 0 */

    uint hash() const;

    /** returns if the guid is a hash or an ID taken from the source */

    bool guidIsHash() const;

    bool guidIsPermaLink() const;

    QDateTime pubDate() const;

    QUrl commentsLink() const;

    int comments() const;

    QString authorName() const;
    QString authorUri() const;
    QString authorEMail() const;
    QString authorAsHtml() const;
    QString authorShort() const;

    QSharedPointer<const Syndication::Enclosure> enclosure() const;

    bool operator<(const Article &other) const;
    bool operator<=(const Article &other) const;
    bool operator>(const Article &other) const;
    bool operator>=(const Article &other) const;

private: //only for our friends
    void setStatus(int s);
    void setDeleted();
    void setKeep(bool keep);

private:
    struct Private;
    Private *d;
};
} // namespace Akregator
Q_DECLARE_TYPEINFO(Akregator::Article, Q_MOVABLE_TYPE);
#endif // AKREGATOR_ARTICLE_H
