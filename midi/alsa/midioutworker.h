/* 
 * File:   MidiOutWorker.h
 * Author: jrussell
 *
 * Created on September 11, 2013, 9:15 PM
 */

#ifndef MIDIOUTWORKER_H
#define	MIDIOUTWORKER_H
#include <QThread>
#include <QtCore>
#include "alsa/asoundlib.h"

#define CHUNKSIZE 256

class MidiOutWorker :public QObject {
    Q_OBJECT
public:
    MidiOutWorker(snd_seq_t* sequencerHandle, int port); //message?
    MidiOutWorker(const MidiOutWorker& orig);
    virtual ~MidiOutWorker();
signals:
    void progress(int complete, int total, int completeId);
    void sysexComplete(int completeId);
    void error(QString message);
public slots:
    //Do the work
    void sendSysex(QByteArray message, int completeId);
private:
    snd_seq_t* sequencerHandle;
    int port;
};

#endif	/* MIDIOUTWORKER_H */

