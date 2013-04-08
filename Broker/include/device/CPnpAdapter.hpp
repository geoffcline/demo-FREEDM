////////////////////////////////////////////////////////////////////////////////
/// @file           CPnpAdapter.hpp
///
/// @author         Thomas Roth <tprfh7@mst.edu>
///
/// @project        FREEDM DGI
///
/// @description    Adapter for plug-and-play devices on an ARM board.
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

#ifndef C_PNP_ADAPTER_HPP
#define C_PNP_ADAPTER_HPP

#include "IBufferAdapter.hpp"
#include "CTcpServer.hpp"

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace freedm {
namespace broker {
namespace device {

/// ARM adapter that maintains a TCP server for a set of plug-and-play devices.
////////////////////////////////////////////////////////////////////////////////
/// The ARM adapter implements the plug-and-play session protocol. An object of
/// this class will delete itself on expiration of an internal countdown timer
/// unless the CPnpAdapter::Heartbeat function is called to refresh the timer.
/// The adapter calls this function itself whenever it sends or receives data
/// on its TCP server without incident.
///
/// @limitations A shared pointer to this class must never be stored except in
/// the CAdapterFactory class. If a shared pointer is stored, then the session
/// protocol will no longer function as intended.
////////////////////////////////////////////////////////////////////////////////
class CPnpAdapter
    : public IBufferAdapter
{
public:
    /// Convenience type for a shared pointer to self.
    typedef boost::shared_ptr<CPnpAdapter> Pointer;

    /// Creates a shared instance of the arm adapter.
    static IAdapter::Pointer Create(boost::asio::io_service & service,
            boost::property_tree::ptree & p, CTcpServer::Connection client);

    /// Starts the internal countdown timer.
    void Start();

    /// Refreshes the internal countdown timer.
    void Heartbeat();

    /// Destructs the object.
    virtual ~CPnpAdapter();
private:
    /// Initializes the TCP server and internal storage.
    CPnpAdapter(boost::asio::io_service & service,
            boost::property_tree::ptree & p, CTcpServer::Connection client);

    /// Tells the adapter factory to remove its reference to this object.
    void Timeout(const boost::system::error_code & e);

    void StartRead();

    void StartWrite();

    void HandleRead(const boost::system::error_code & e);

    void HandleWrite(const boost::system::error_code & e);

    /// Parses a state packet received from the client.
    void ReadStatePacket(const std::string packet);

    /// Sends device commands to the current client.
    std::string GetCommandPacket();

    /// Countdown until the object destroys itself.
    boost::asio::deadline_timer m_countdown;

    /// Unique identifier of this adapter.
    std::string m_identifier;

    /// TCP server for the ARM client.
    CTcpServer::Connection m_client;

    bool m_stop;

    boost::asio::streambuf m_buffer;
};

} // namespace device
} // namespace broker
} // namespace freedm

#endif // C_PNP_ADAPTER_HPP
