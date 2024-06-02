let cardVariants = {
  "toHidden": {
    "transform": [|
      "translateY(0px) rotateY(0deg)",
      "translateY(-10px) rotateY(0deg)",
      "translateY(-10px) rotateY(180deg)",
      "translateY(0px) rotateY(180deg)",
    |],
    "transition": {
      "duration": 0.6,
      "times": [|0.0, 0.16, 0.84, 1.0|],
    },
  },
  "toRevealed": {
    "transform": [|
      "translateY(0px) rotateY(180deg)",
      "translateY(-10px) rotateY(180deg)",
      "translateY(-10px) rotateY(360deg)",
      "translateY(0px) rotateY(360deg)",
    |],
    "transition": {
      "duration": 0.6,
      "times": [|0.0, 0.16, 0.84, 1.0|],
    },
  },
  "toDeleted": {
    "transform": [|"translateY(-50px)"|],
  },
};

let toRevealed = Js.Nullable.return("toRevealed");
let toHidden = Js.Nullable.return("toHidden");
let null = Js.Nullable.null;

type revealState =
  | Initial
  | Revealed
  | Hidden;

let useRevealState = () => {
  let (state, reduce) =
    React.useReducer(
      (state, ()) =>
        switch (state) {
        | Initial => Revealed
        | Revealed => Hidden
        | Hidden => Revealed
        },
      Initial,
    );

  // faceInitial, faceAnimate, backInitial, backAnimate
  switch (state) {
  | Initial => (toRevealed, null, toHidden, null, reduce)
  | Revealed => (null, toRevealed, null, toHidden, reduce)
  | Hidden => (null, toHidden, null, toRevealed, reduce)
  };
};
