/*
  Copyright (c) 2016 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "grantleeviewformatter.h"
#include "articlegrantleeobject.h"
#include "utils.h"
#include <KLocalizedString>

#include <grantlee/engine.h>

#include <QVariantHash>
#include <QApplication>
#include <QDateTime>
#include <QVariantList>
#include <QDebug>

using namespace Akregator;

GrantleeViewFormatter::GrantleeViewFormatter(const QString &htmlFileName, const QString &themePath, const QUrl &imageDir, QObject *parent)
    : PimCommon::GenericGrantleeFormatter(htmlFileName, themePath, parent),
      mImageDir(imageDir)
{
}

GrantleeViewFormatter::~GrantleeViewFormatter()
{

}

QString GrantleeViewFormatter::formatArticle(const QVector<Article> &article, ArticleFormatter::IconOption icon) const
{
    if (!errorMessage().isEmpty()) {
        return errorMessage();
    }

    QVariantHash articleObject;

    QVariantList articlesList;
    const int nbArticles(article.count());
    articlesList.reserve(nbArticles);
    for (int i = 0; i < nbArticles; ++i) {
        ArticleGrantleeObject *articleObj = new ArticleGrantleeObject(mImageDir, article.at(i), icon);
        articlesList << QVariant::fromValue(static_cast<QObject *>(articleObj));
    }
    articleObject.insert(QStringLiteral("articles"), articlesList);

    const QString directionString = QApplication::isRightToLeft() ? QStringLiteral("rtl") : QStringLiteral("ltr");
    articleObject.insert(QStringLiteral("applicationDir"), directionString);

    articleObject.insert(QStringLiteral("dateI18n"), i18n("Date"));
    articleObject.insert(QStringLiteral("commentI18n"), i18n("Comment"));
    articleObject.insert(QStringLiteral("completeStoryI18n"), i18n("Complete Story"));
    articleObject.insert(QStringLiteral("authorI18n"), i18n("Author"));
    articleObject.insert(QStringLiteral("enclosureI18n"), i18n("Enclosure"));
    return render(articleObject);
}

