#ifndef COOLFluid_server_ServerNetworkComm_h
#define COOLFluid_server_ServerNetworkComm_h

////////////////////////////////////////////////////////////////////////////////

#include <QObject>
#include <QAbstractSocket>
#include <QDomDocument>
#include <QList>
#include <QMutex>

#include "GUI/Network/HostInfos.hpp"
#include "GUI/Network/ComponentType.hpp"

class QHostAdress;
class QTcpServer;
class QTcpSocket;
class QString;

///////////////////////////////////////////////////////////////////////////////

#include "Common/XmlHelpers.hpp"

namespace CF {

namespace GUI {

namespace Network
{
  class ComponentType;
  struct FrameInfos;
}

namespace Server {

/////////////////////////////////////////////////////////////////////////////

  /// @brief This class is the server network level.

  /// For all signals emitted by this class, the first parameter identifies the
  /// client that caused the signal to be emitted. A same client will always
  /// have the same id. When client disconnects, its id will never be given
  /// to another client.

  /// @author Quentin Gasper.
  class ServerNetworkComm : public QObject
  {
    Q_OBJECT

  public:

    /// @brief Constructor.

    ServerNetworkComm();

    /// @brief Destructor.

    /// Closes the sockets before the object is deleted.
    ~ServerNetworkComm();

    /// @brief Opens a port the server has to listen to.

    /// @param hostAddress Server address.
    /// @param port Socket port.
    /// @throw NetworkException Throws a NetworkException if the server cannot
    /// listen to the given address/port.
    bool openPort(const QString & hostAddress = "127.0.0.1", quint16 port = 62784);

    /// @brief Gives the number of bytes recieved.

    /// @return Returns the number of bytes recieved.
    int getBytesRecieved() const;

    /// @brief Gives the number of bytes sent.

    /// @return Returns the number of bytes sent.
    int getBytesSent() const;

    /// @brief Sends an error message to a client

    /// @param message Message to send
    /// @param uuid Client UUID, or an empty string to send to all clients.
    /// @throw UnknownClientIdException if Client UUID is unknown.
    void sendErrorToClient(const QString & message, const std::string & uuid = std::string());

    /// @brief Sends a message to a client

    /// @param message Message to send
    /// @param uuid Client UUID, or an empty string to send to all clients.
    /// @throw UnknownClientIdException if Client UUID is unknown.
    void sendMessageToClient(const QString & message, const std::string & uuid = std::string());

    void sendSignalToClient(const CF::Common::XmlNode & signal,
                            const std::string & uuid = std::string());

    void sendFrameRejectedToClient(const std::string clientid,
                                   const std::string & frameid,
                                   const CF::Common::CPath & sender,
                                   const QString & reason);

    void disconnectAll();


  private slots :

    /// @brief Slot called when a new client tries to connect.
    void newClient();

    /// @brief Slot called when new data are available on one of the
    /// client sockets.
    void newData();

    /// @brief Slot called when the client has been disconnected.
    void clientDisconnected();

    /// @brief Slot called when a message (i.e. output forwarding) comes
    /// from the simulator.

    /// Sends this message to all clients.
    /// @param message The message.
    void message(const QString & message);

    /// @brief Slot called when an error comes from the simulator.

    /// Sends this error message to all clients.
    /// @param message The error message.
    void error(const QString & message);

  signals:

    void newClientConnected(const std::string & uuid);

  private:

    /// @brief The server m_socket.

    /// Used to accept connections.
    QTcpServer * m_server;

    /// @brief The server m_socket for the local loop.

    /// Used to accept connections coming from "localhost" (local loop).
    QTcpServer * m_localSocket;

    /// @brief Size of the frame that is being read.

    /// If the value is 0, no frame is currently being recieved.
    quint32 m_blockSize;

    /// @brief The client sockets.

    /// The key is pointer to the m_socket. The value is the client UUID.
    QHash<QTcpSocket *, std::string> m_clients;

    /// @brief Number of bytes recieved.
    int m_bytesRecieved;

    /// @brief Number of bytes sent.
    int m_bytesSent;

    /// @brief Last ID given to a client
    int m_lastClientId;

    /// @brief Sends a message to a client.

    /// @param client Client socket to use. If @c CFNULL, the message will be
    /// sent to all clients.
    /// @param message Message to send.
    /// @return Returns @c true if the frame was built and sent successfully;
    /// otherwise returns @c false.
    bool sendMessage(QTcpSocket * client, const QString & message);

    /// @brief Sends an error message to a client.

    /// @param client Client socket to use. If @c CFNULL, the error message will
    /// be sent to all clients.
    /// @param message Error message to send.
    /// @return Returns @c true if the frame was built and sent successfully;
    /// otherwise returns @c false.
    bool sendError(QTcpSocket * client, const QString & message);

    /// @brief Sends a message to a client.

    /// @param client Client m_socket to use. If @c CFNULL, the frame will be sent
    /// to all clients.
    /// @param signal Signal frame to send.
    /// @return Returns the number of bytes sent.
    int send(QTcpSocket * client, const CF::Common::XmlNode & signal);

		bool sendFrameRejected(QTcpSocket * client,
													 const std::string & frameid,
													 const CF::Common::CPath & sender,
													 const QString & reason);

		/// @brief Retrieves a client socket from its UUID.

    /// @param uuid Client UUID
    /// @return Returns a pointer to the socket, or @c CFNULL if client
    /// UUID was -1 (all clients).
    /// @throw UnknownClientIdException if Client id is unknown.
    QTcpSocket * getSocket(const std::string & uuid) const;

		std::string getAttr(const CF::Common::XmlNode & node,
												const char * paramName,
												QString & reason);

	}; // class ServerNetworkComm

////////////////////////////////////////////////////////////////////////////

} // namespace Server
} // namespace GUI
} // namespace CF

////////////////////////////////////////////////////////////////////////////////

#endif // COOLFluid_server_ServerNetworkComm_h
