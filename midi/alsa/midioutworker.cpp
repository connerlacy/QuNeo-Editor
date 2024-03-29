/* 
 * File:   MidiOutWorker.cpp
 * Author: J. Russell Smyth (jrussell.smyth@gmail.com)
 * 
 * Created on September 11, 2013, 9:15 PM
 */

#include "midioutworker.h"

MidiOutWorker::MidiOutWorker(snd_seq_t* sequencerHandle, int port) :sequencerHandle(sequencerHandle), port(port){
}

MidiOutWorker::~MidiOutWorker() {
}

void MidiOutWorker::sendSysex(QByteArray message, int correlationId) {
    snd_seq_event_t event;
    // init the event
    snd_seq_ev_clear(&event);
    // set event source to output port
    snd_seq_ev_set_source(&event, port);
    // set to braodcast to subscribers to source port
    snd_seq_ev_set_subs(&event);
    // set for direct (non-queued) delivery
    snd_seq_ev_set_direct(&event);
    int bytesSent = 0;

    int err;
    std::vector<unsigned char> chunk;

    for (unsigned int i = 0; i < message.size(); i++) {
        chunk.push_back(message.at(i));
        if (chunk.size() == CHUNKSIZE || chunk.back() == 0xF7) {
            snd_seq_ev_set_sysex(&event, chunk.size(), &chunk.front());
            if ((err = snd_seq_event_output_direct(sequencerHandle, &event)) < 0) {
                QString errorMessage = QString("Error occurred:%s").arg(snd_strerror(err));
                emit error(errorMessage);
                qDebug() << errorMessage;
                return;
            }
            bytesSent += chunk.size();
            qDebug("emitting progress %d %d %d", bytesSent, message.size(), correlationId);
            emit progress(bytesSent, message.size(), correlationId);
            usleep(chunk.size() * 1024 /*352*/);
            chunk.clear();
        }
    }
    qDebug("emitting sysexComplete");
    emit sysexComplete(correlationId);
}

