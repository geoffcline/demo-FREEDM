////////////////////////////////////////////////////////////////////////////////
/// @file           CTcpServer.hpp
///
/// @author         Thomas Roth <tprfh7@mst.edu>
///
/// @project        FREEDM DGI
///
/// @description    TCP server that accepts a single client connection.
///
/// These source code files were created at Missouri University of Science and
/// Technology, and are intended for use in teaching or research. They may be
/// freely copied, modified, and redistributed as long as modified versions are
/// clearly marked as such and this notice is not removed. Neither the authors
/// nor Missouri S&T make any warranty, express or implied, nor assume any legal
/// responsibility for the accuracy, completeness, or usefulness of these files
/// or any information distributed with these files.
///
/// Suggested modifications or questions about these files can be directed to
/// Dr. Bruce McMillin, Department of Computer Science, Missouri University of
/// Science and Technology, Rolla, MO 65409 <ff@mst.edu>.
////////////////////////////////////////////////////////////////////////////////

#ifndef C_TCP_SERVER_HPP
#define C_TCP_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace freedm {
namespace broker {
namespace device {

/// TCP server that handles a single client connection.
////////////////////////////////////////////////////////////////////////////////
/// A TCP server that redirects clients to the registered connection handler.
///
/// @limitations None.
////////////////////////////////////////////////////////////////////////////////
class CTcpServer
    : private boost::noncopyable
{
public:
    /// Convenience type for a shared pointer to self.
    typedef boost::shared_ptr<CTcpServer> Pointer;
    
    /// Type of the callback function for client connections.
    typedef boost::function<void (boost::asio::ip::tcp::socket &)> Callback;
    
    /// Stops the TCP server.
    virtual ~CTcpServer();
    
    /// Creates a new TCP server on the specified port number.
    static Pointer Create(boost::asio::io_service & ios, unsigned short port);
    
    /// Registers a callback function for client connections.
    void RegisterHandler(Callback h);
    
    /// Gets the listen port of the TCP server.
    unsigned short GetPort() const;
    
    /// Gets the hostname of the connected client.
    std::string GetHostname() const;
private:
    /// Constructs the TCP server on the specified port number.
    CTcpServer(boost::asio::io_service & ios, unsigned short port);
    
    /// Prepares to accept the next client.
    void StartAccept();
    
    /// Handles an accepted client connection.
    void HandleAccept(const boost::system::error_code & error);
    
    /// Gets a log header.
    std::string hdr() const;
    
    /// Acceptor for new client connections.
    boost::asio::ip::tcp::acceptor m_acceptor;
    
    /// Port number of the server.
    unsigned short m_port;
    
    /// Callback function to handle clients.
    ConnectionHandler m_handler;
protected:
    /// Socket for the current client.
    mutable boost::asio::ip::tcp::socket m_socket;
};

} // namespace device
} // namespace broker
} // namespace freedm

#endif // C_TCP_SERVER_HPP
