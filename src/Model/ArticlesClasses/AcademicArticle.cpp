#include "AcademicArticle.h"

AcademicArticle::AcademicArticle(const unsigned int id, const QString &title, const QString &short_description, const QString &image_path,
        const QString &author, const QString &category, const QString &url, const QDate &date, const unsigned int word_count,
        const QString &university_name, const unsigned int citation_count, const bool peer_reviewed)
        : Articles(id, title, short_description, image_path, author, category, url, date, word_count),
          universityName(university_name),
          citationCount(citation_count),
          peerReviewed(peer_reviewed) {
}

void AcademicArticle::accept(Visitor *visitor) {
    visitor->visit(this);
}

QString AcademicArticle::getUniversityName() const {
    return universityName;
}

unsigned int AcademicArticle::getCitationCount() const {
    return citationCount;
}

bool AcademicArticle::isPeerReviewed() const {
    return peerReviewed;
}

void AcademicArticle::setCitationCount(unsigned int citation_count) {
    citationCount = citation_count;
}

void AcademicArticle::setPeerReviewed(bool peer_reviewed) {
    peerReviewed = peer_reviewed;
}

void AcademicArticle::setUniversityName(const QString &university_name) {
    universityName = university_name;
}
