/* 
 * File:   MidiOutWorker.h
 * Author: J. Russell Smyth (jrussell.smyth@gmail.com)
 *
 * Created on September 11, 2013, 9:15 PM
 */

#ifndef MIDIOUTWORKER_H
#define	MIDIOUTWORKER_H
#include <QThread>
#include <QtCore>
#include "alsa/asoundlib.h"

#define CHUNKSIZE 16

/**
 * @class
 * @brief Worker to send MIDI data
 * MidiOutWorker should be run on a QThread and MIDI messages sent by message to the sendSysex slot.
 * Note that MidiOutWorker does NOT own the sequencer handle or port and does not clean them up after use.
 */
class MidiOutWorker :public QObject {
    Q_OBJECT
public:
    /**
     * @brief MidiOutWorker
     * @param sequencerHandle handle to the alsa sequencer
     * @param port port id to write to
     */
    MidiOutWorker(snd_seq_t* sequencerHandle, int port);
    MidiOutWorker(const MidiOutWorker& orig): sequencerHandle(orig.sequencerHandle), port(orig.port){}
    virtual ~MidiOutWorker();
signals:
    /**
     * @brief emitted as each @see CHUNKSIZE block is sent for large sysex
     * @param complete
     * @param total
     * @param correlationId the correlation id supplied with the associated sendSysex signal
     */
    void progress(int complete, int total, int correlationId);
    /**
     * @brief emitted when a sysex message is fully sent to alsa
     * @param correlationId the correlation id supplied with the associated sendSysex signal
     */
    void sysexComplete(int correlationId);
    void error(QString message);
public slots:
    /**
     * @brief transmit a sysex message
     * @param message
     * @param correlationId an integer id for status signal correlation
     */
    void sendSysex(QByteArray message, int correlationId);
private:
    snd_seq_t* sequencerHandle;
    int port;
};

#endif	/* MIDIOUTWORKER_H */

