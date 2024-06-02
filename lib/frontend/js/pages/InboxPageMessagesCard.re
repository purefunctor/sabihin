let containerCss = [%cx
  {|
  display: flex;
  position: relative;
  width: 100%;
  height: 16rem;
|}
];

let cardCss = [%cx
  {|
  height: 100%;
  width: 100%;
  position: absolute;
  backface-visibility: hidden;
  color: $(Theme.background11);
  font-family: "Poppins";
  font-size: 4rem;
  cursor: pointer;
|}
];

let backCss = [%cx {|
  background-color: $(Theme.primary);
|}];

let faceCss = [%cx {|
  background-color: $(Theme.secondary);
|}];

let variants = {
  "toHidden": {
    "transform": [|
      "perspective(1000px) translateY(0px) rotateY(0deg)",
      "perspective(1000px) translateY(-10x) rotateY(0deg)",
      "perspective(1000px) translateY(-10px) rotateY(180deg)",
      "perspective(1000px) translateY(0px) rotateY(180deg)",
    |],
    "transition": {
      "duration": 0.6,
      "times": [|0.0, 0.16, 0.84, 1.0|],
    },
  },
  "toRevealed": {
    "transform": [|
      "perspective(1000px) translateY(0px) rotateY(180deg)",
      "perspective(1000px) translateY(-10px) rotateY(180deg)",
      "perspective(1000px) translateY(-10px) rotateY(360deg)",
      "perspective(1000px) translateY(0px) rotateY(360deg)",
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

type state =
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

  let toRevealed = Js.Nullable.return("toRevealed");
  let toHidden = Js.Nullable.return("toHidden");
  let null = Js.Nullable.null;

  // faceInitial, faceAnimate, backInitial, backAnimate
  switch (state) {
  | Initial => (toRevealed, null, toHidden, null, reduce)
  | Revealed => (null, toRevealed, null, toHidden, reduce)
  | Hidden => (null, toHidden, null, toRevealed, reduce)
  };
};

[@react.component]
let make = () => {
  let (faceInitial, faceAnimate, backInitial, backAnimate, toggleReveal) =
    useRevealState();

  let onClick = React.useCallback1(_ => toggleReveal(), [|toggleReveal|]);

  <div className=containerCss onClick>
    <FramerMotion.div
      className={cardCss ++ " " ++ faceCss}
      variants
      initial=faceInitial
      animate=faceAnimate>
      {React.string("Face")}
    </FramerMotion.div>
    <FramerMotion.div
      className={cardCss ++ " " ++ backCss}
      variants
      initial=backInitial
      animate=backAnimate>
      {React.string("Back")}
    </FramerMotion.div>
  </div>;
};
