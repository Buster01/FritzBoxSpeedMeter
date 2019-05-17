void refreshNonce(void){
  // Do a dummy request which will result in an authentication error, but will refresh the nonce
  String req[][2] = {{"NewAssociatedDeviceIndex", "1"}};
  TR064connection.action("urn:dslforum-org:service:WLANConfiguration:1", "GetGenericAssociatedDeviceInfo", req, 1);
}

int getDeviceNumber() {
  String params[][2] = {{}};
  String req[][2] = {{"NewHostNumberOfEntries", ""}};
  TR064connection.action("urn:dslforum-org:service:Hosts:1", "GetHostNumberOfEntries", params, 0, req, 1);
  int numDev = (req[0][1]).toInt();
  return numDev;
}

float getUpstreamMaxBitRate() {
  String params[][2] = {{}};
  String req[][2] = {{"NewLayer1UpstreamMaxBitRate", ""}};
  TR064connection.action("urn:dslforum-org:service:WANCommonInterfaceConfig:1", "GetCommonLinkProperties", params, 0, req, 1);
  long numDev = req[0][1].toInt();
  Serial.println("MaxUpload: " + String(numDev / factor2));
  return (numDev / factor2);
}

float getDSLDownstreamMaxBitRate() {
  String params[][2] = {{}};
  String req[][2] = {{"NewLayer1DownstreamMaxBitRate", ""}};
  TR064connection.action("urn:dslforum-org:service:WANCommonInterfaceConfig:1", "GetCommonLinkProperties", params, 0, req, 1);
  long numDev = (req[0][1]).toInt();
  Serial.println ("MaxDownload: " + String(numDev / factor2));
  return (numDev / factor2);
}

float getNewByteUploadRate() {
  String params[][2] = {{"NewSyncGroupIndex", "0"}};
  String req[][2] = {{"Newus_current_bps", ""}};
  TR064connection.action("urn:dslforum-org:service:WANCommonInterfaceConfig:1", "X_AVM-DE_GetOnlineMonitor",params, 1, req, 1);
  long numDev = req[0][1].toInt();
  Serial.println("Upload: " + String((numDev*8) / factor1));
  return ((numDev*8) / factor1);
}

float getNewByteDownloadRate() {
  String params[][2] = {{"NewSyncGroupIndex", "0"}};
  String req[][2] = {{"Newds_current_bps", ""}};
  TR064connection.action("urn:dslforum-org:service:WANCommonInterfaceConfig:1", "X_AVM-DE_GetOnlineMonitor",params, 1, req, 1);
  long numDev = req[0][1].toInt();
  Serial.println("Download: " + String((numDev*8) / factor1));
  return ((numDev*8) / factor1);
}
