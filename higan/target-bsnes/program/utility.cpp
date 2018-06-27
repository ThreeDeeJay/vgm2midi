auto Program::showMessage(string text) -> void {
  statusTime = chrono::timestamp();
  statusMessage = text;
}

auto Program::showFrameRate(string text) -> void {
  statusFrameRate = text;
}

auto Program::updateStatus() -> void {
  string message;
  if(chrono::timestamp() - statusTime <= 2) {
    message = statusMessage;
  }
  message.prepend("  ");
  if(message != presentation->statusLeft.text()) {
    presentation->statusLeft.setText(message);
  }

  string frameRate;
  if(!emulator->loaded()) {
    frameRate = "Unloaded";
  } else if(presentation->pauseEmulation.checked()) {
    frameRate = "Paused";
  } else if(!focused() && settingsWindow->input.pauseEmulation.checked()) {
    frameRate = "Paused";
  } else {
    frameRate = statusFrameRate;
  }
  frameRate.append("  ");
  if(frameRate != presentation->statusRight.text()) {
    presentation->statusRight.setText(frameRate);
  }
}

auto Program::focused() -> bool {
  //exclusive mode creates its own top-level window: presentation window will not have focus
  if(video && video->exclusive()) return true;
  if(presentation && presentation->focused()) return true;
  return false;
}