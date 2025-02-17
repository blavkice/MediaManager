#ifndef ACADEMICARTICLE_H
#define ACADEMICARTICLE_H
#include "../Articles.h"

class AcademicArticle : public Articles {
private:
    QString universityName;
    unsigned int citationCount;
    bool peerReviewed;
public:
    AcademicArticle() = default;
    AcademicArticle(const unsigned int id, const QString &title, const QString &short_description, const QString &image_path,
        const QString &author, const QString &category, const QString &url, const QDate &date, const unsigned int word_count,
        const QString &university_name, const unsigned int citation_count, const bool peer_reviewed);
    QString getUniversityName() const;
    unsigned int getCitationCount() const;
    bool isPeerReviewed() const;
    void setCitationCount(unsigned int citation_count);
    void setPeerReviewed(bool peer_reviewed);
    void setUniversityName(const QString &university_name);
};

#endif //ACADEMICARTICLE_H
