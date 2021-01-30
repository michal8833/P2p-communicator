#include <contacts/Message.h>

using namespace contacts;

Message::Message(TCPPacket packet, QObject* parent)
        : QObject(parent){
    log = Logger(Config::config("log-file"), Config::config().debugMode());
    this->type = packet.getType() == TCPPacket::PacketType::TEXT
            ? Message::Type::TEXT
            : Message::Type::FILE;
    this->timestamp = QDateTime::currentDateTime();
    this->content = packet.getContent();
    this->filename = packet.getFilename();
}

Message::Message(QJsonObject& object QObject* parent)
        : QObject(parent) {
    log = Logger(Config::config("log-file"), Config::config().debugMode());
    this->type = object["type"].toString().toStdString() == "TEXT"
            ? Message::Type::TEXT
            : Message::Type::FILE;
    this->filename = object["filename"].toString().toStdString();
    this->content = object["content"].toString().toStdString();
    this->timestamp = QDateTime::fromString(object["timestamp"].toString());
    this->address = object["address"].toString().toStdString();
}

QJsonObject Message::serialize() {
    QJsonObject object{};
    object["type"] = getType() == Message::Type::TEXT
            ? QString("TEXT")
            : QString("FILE");
    object["timestamp"] = QString(getTimestamp().c_str());
    if (getType() == Message::Type::TEXT) {
        object["content"] = QString(getContent().c_str());
    } else {
        object["filename"] = QString(getFilename().c_str());
    }
    return object;
}

void Message::save() {
    if (type == FILE) {
        QDir dir;
        QString dirPath = Config::config("downloads-directory").c_str();
        if(!dir.exists(dirPath)) {
            dir.mkpath(dirPath);
        }

        QString filename = QString(dirPath + QString::fromStdString("/") + QString::fromStdString(getFilename()));

        ofstream file(filename.toUtf8());
        if (!file.is_open()) {
            throw new IOException("Failed saving file.");
        }
        file << content;
        file.close();

        log.info("Saved file '" + filename.toStdString() + "'");
    }
}

string Message::getTimestamp() const {
    return timestamp.toString().toStdString();
}
