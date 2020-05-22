#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID steerPID; // PID for steerValue
  PID throttlePID; // PID for throttleValue to control speed
  //======================//
  // tune these variables //
  //======================//
  steerPID.Init(0.1,0.001,1.0);
  throttlePID.Init(0.1,0.00,0.1);

  h.onMessage([&steerPID, &throttlePID](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());

          //========================================== //
          // implement the PID code for steering angle //
          //========================================== //
          double steerValue;
          // Update the Errors at each step & get the Total Values
          steerPID.UpdateError(cte);
          steerValue = -steerPID.TotalError();
          // maximum & minimum steerValue set at 1 and  -1
          if (steerValue    < -1) {steerValue = -1;}
          if (steerValue    >  1) {steerValue =  1;}

          //============================================ //
          // implement the PID code for throttle control //
          //============================================ //
          double throttleValue;
          // Update the Steering Angle Errors at each step & get the Total Values
          throttlePID.UpdateError(fabs(angle));
          throttleValue = 0.7 - throttlePID.TotalError();
          // just a failsafe to make sure the car doesn't reverse on track
          if (speed < 20){
            throttleValue = std::max(0.4, throttleValue);
          }

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steerValue
                    << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steerValue;
          // msgJson["throttle"] = 0.4;
          msgJson["throttle"] = throttleValue;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }

  h.run();
}
