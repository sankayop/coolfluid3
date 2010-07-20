
#ifndef CF_GUI_Client_NMethod_hpp
#define CF_GUI_Client_NMethod_hpp

//////////////////////////////////////////////////////////////////////////////

#include <QObject>

#include "GUI/Client/CNode.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace GUI {
namespace Client {

  ////////////////////////////////////////////////////////////////////////////

  /// @brief Client corresponding component for @c CF::Mesh::CMesh.
  class NMethod :
      public QObject,
      public CNode
  {
    Q_OBJECT

  public:

    typedef boost::shared_ptr<NMethod> Ptr;
    typedef boost::shared_ptr<NMethod const> ConstPtr;

    /// @brief Constructor
    /// @param name Node name
    NMethod(const QString & name);

    /// @brief Gives the icon associated to this node
    /// @return Returns the icon associated to this node
    /// @note This method should be reimplemented by all subclasses.
    virtual QIcon getIcon() const;

    /// @brief Gives the text to put on a tool tip
    /// @return The name of the class.
    virtual QString getToolTip() const;

    /// @brief Gives node options.
    /// @param params Reference to a list where options will be put. The
    /// list is cleared before first use.
    virtual void getOptions(QList<NodeOption> & params) const;

    /// @brief Indicates whether this class is a client component or not
    /// @return Always returns @c false.
    virtual bool isClientComponent() const { return false; }

  private:
    /// regists all the signals declared in this class
    static void regist_signals ( Component* self ) {}


  }; // class NMethod

//////////////////////////////////////////////////////////////////////////////

} // namespace Client
} // namespace GUI
} // namespace CF

//////////////////////////////////////////////////////////////////////////////

#endif // CF_GUI_Client_NMethod_hpp
