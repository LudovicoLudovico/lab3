#ifndef NUMS
#define NUMS

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

// ground_fondoscala (5 if mV, 0.005 if V)
void calc_err(std::string instrument, Double_t* data, Double_t* err,
              int N_POINTS, double ground_fondoscala) {
  for (int i = 0; i < N_POINTS; i++) {
    if (instrument == "OSCILL") {  // Error on Oscilloscope (Voltage)
      const double TACCHETTE_APPREZZABILI = 0.5;
      err[i] = sqrt(pow(err[i] * TACCHETTE_APPREZZABILI / 5., 2) +
                    pow(ground_fondoscala * TACCHETTE_APPREZZABILI / 5., 2) +
                    pow(data[i] * 0.03, 2));
    } else if (instrument == "MULT") {  // Error on multimeter (Current)
      err[i] =
          static_cast<float>(static_cast<int>(data[i] * 0.015 * 1e4 + 0.5)) /
              1e4 +
          0.02;
    } else {
      std::cout << "****** \n INVALID INSTRUMENT \n ******";
    }
  }
}

vector<string> approx(double value, double err) {
  std::stringstream stream;
  stream << std::setprecision(1) << err;
  std::string err_str = stream.str();
  stream.str("");

  int counter = 0;
  int power = 0;
  bool has_found_period = false;

  for (int i = 0; i < err_str.length(); i++) {
    if (err_str[i] == '.') {
      has_found_period = true;
      continue;
    }
    if (err_str[i] == 'e') {
      power = std::stoi(err_str.substr(i + 1, err_str.length()));
      break;
    }
    if (has_found_period) {
      counter += 1;
    }
  }

  double pow = 1.;
  double approx = 0.5;
  if (value < 0) approx *= -1;

  if (power == 0) {
    for (int i = 0; i < counter; i++) {
      pow *= 10;
    }

    std::stringstream val;
    val << std::fixed << std::setprecision(counter)
        << static_cast<float>(static_cast<int>(value * pow + approx)) / pow;

    vector<string> output = {val.str(), err_str};
    return output;
  } else {
    for (int i = 0; i < counter + 1; i++) {
      pow /= 10.;
    }
    value = static_cast<float>(static_cast<int>(value * pow + approx));
    std::string pow_str = "";
    if (power > 1) {
      pow_str = "^{" + std::to_string(power) + "}";
    }

    stringstream err;
    err << err_str.substr(0, err_str.length() - 4) << ") \\cdot 10";
    stringstream val;
    val << "(" << value;
    vector<string> output = {val.str(), err.str()};
    return output;
  }
}
#endif