type optionOrError('a) =
  | Some('a)
  | None
  | Error;

type state = {
  weather: optionOrError(WeatherData.weather)
};

type action =
  | WeatherLoaded(WeatherData.weather)
  | WeatherError;

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  ...component,

  initialState: () => {
    weather: None
  },

  didMount: (self) => {
    let handleWeatherLoaded = weather => self.send(WeatherLoaded(weather));
    let handleWeatherError = () => self.send(WeatherError);

    WeatherData.getWeather()
      |> Js.Promise.then_(
        weather => {
          handleWeatherLoaded(weather);
          Js.Promise.resolve();
        }
      )
      |> Js.Promise.catch(
        _err => {
          handleWeatherError();
          Js.Promise.resolve();
        }
      )
      |> ignore;

    ReasonReact.NoUpdate;
  },

  reducer: (action, _prevState) => {
    switch action {
    | WeatherLoaded(newWeather) =>
      ReasonReact.Update({
        weather: Some(newWeather)
      })
    | WeatherError =>
      ReasonReact.Update({
        weather: Error
      })
    }
  },

  render: (self) =>
    <div className="App">
      <p>
      {
        switch self.state.weather {
        | None =>
            ReasonReact.stringToElement("Loading weather...");
        | Error =>
            ReasonReact.stringToElement("Error loading weather.");
        | Some(weather) =>
            ReasonReact.stringToElement(weather.summary);
        }
      }
      </p>
    </div>
};
