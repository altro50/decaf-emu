#pragma once
#include "debugger_interface.h"
#include "debugger_server.h"
#include "debugger_ui_state.h"

#include <common/platform.h>
#include <common/platform_socket.h>
#include <string>
#include <vector>
#include <spdlog/spdlog.h>

namespace debugger
{

class GdbServer : public DebuggerServer
{
   static constexpr platform::Socket InvalidSocket = static_cast<platform::Socket>(-1);

   enum class ReadState
   {
      ReadStart,
      ReadCommand,
      ReadChecksum
   };

public:
   GdbServer(DebuggerInterface *debugger, ui::StateTracker *uiState);
   virtual ~GdbServer() = default;

   virtual bool start(int port) override;
   virtual void process() override;

   void closeServer();
   void closeClient();

   void sendAck();
   void sendNack();
   void sendCommand(const std::string &command);

   void handleCommand(const std::string &command);
   void handleBreak();
   void handleQuery(const std::string &command);
   void handleEnableExtendedMode(const std::string &command);
   void handleGetHaltReason(const std::string &command);
   void handleReadRegister(const std::string &command);
   void handleReadGeneralRegisters(const std::string &command);
   void handleReadMemory(const std::string &command);
   void handleAddBreakpoint(const std::string &command);
   void handleRemoveBreakpoint(const std::string &command);
   void handleVCont(const std::vector<std::string> &command);
   void handleVContQuery(const std::vector<std::string> &command);
   void handleSetActiveThread(const std::string &command);

private:
   std::shared_ptr<spdlog::logger> mLog;

   bool mWasPaused = false;
   uint32_t mLastNia = 0;

   DebuggerInterface *mDebugger = nullptr;
   ui::StateTracker *mUiState = nullptr;

   platform::Socket mListenSocket = InvalidSocket;
   platform::Socket mClientSocket = InvalidSocket;

   ReadState mReadState = ReadState::ReadStart;
   std::string mChecksumBuffer;
   std::string mReadBuffer;
   std::string mLastCommand;

   int32_t mStepSignal = -1;
};

} // namespace debugger
