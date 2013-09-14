/* 
 * File:   MidiOutWorker.cpp
 * Author: jrussell
 * 
 * Created on September 11, 2013, 9:15 PM
 */

#include "midioutworker.h"

MidiOutWorker::MidiOutWorker(snd_seq_t* sequencerHandle, int port) :sequencerHandle(sequencerHandle), port(port){
}

MidiOutWorker::MidiOutWorker(const MidiOutWorker& orig) {
}

MidiOutWorker::~MidiOutWorker() {
}

void MidiOutWorker::sendSysex(QByteArray message, int completeId) {
    snd_seq_event_t event;
    // init the event
    snd_seq_ev_clear(&event);
    // set event source to output port
    snd_seq_ev_set_source(&event, port);
    // set to braodcast to subscribers to source port
    snd_seq_ev_set_subs(&event);
    // set for direct (non-queued) delivery
    snd_seq_ev_set_direct(&event);
    int bytesSent;

    int err;
    std::vector<unsigned char> chunk;

    // loop through the bytes of the message, sending each CHUNKSIZE block
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
            // TODO: update progress dialog if showing?
            //            usleep(chunk.size() * 352);
            usleep(chunk.size() * 128);
            chunk.clear();
            emit progress(bytesSent, message.size());
            qDebug("chunkSent %d", bytesSent);
        }
    }
    emit sysexComplete(completeId);
}

