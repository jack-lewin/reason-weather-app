type state = {
  weather: option(WeatherData.weather)
};

type action =
  | WeatherLoaded(WeatherData.weather);

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  ...component,

  initialState: () => {
    weather: None
  },

  didMount: (self) => {
    let handleWeatherLoaded = weather => self.send(WeatherLoaded(weather));

    WeatherData.getWeather()
      |> Js.Promise.then_(
        weather => {
          handleWeatherLoaded(weather);
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
    }
  },

  render: (self) =>
    <div className="App">
      <p>
      {
        switch self.state.weather {
        | None =>
            ReasonReact.stringToElement("Loading weather...");
        | Some(weather) =>
            ReasonReact.stringToElement(weather.summary);
        }
      }
      </p>
    </div>
};
