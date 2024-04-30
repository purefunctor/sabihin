module BookLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M3 18.5V5a3 3 0 0 1 3-3h14a1 1 0 0 1 1 1v18a1 1 0 0 1-1 1H6.5A3.5 3.5 0 0 1 3 18.5M19 20v-3H6.5a1.5 1.5 0 0 0 0 3zM5 15.337A3.486 3.486 0 0 1 6.5 15H19V4H6a1 1 0 0 0-1 1z"
      />
    </svg>;
  };
};

module CheckLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="m10 15.17l9.192-9.191l1.414 1.414L10 17.999l-6.364-6.364l1.414-1.414z"
      />
    </svg>;
  };
};

module CloseLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="m12 10.587l4.95-4.95l1.414 1.414l-4.95 4.95l4.95 4.95l-1.415 1.414l-4.95-4.95l-4.949 4.95l-1.414-1.415l4.95-4.95l-4.95-4.95L7.05 5.638z"
      />
    </svg>;
  };
};

module InboxLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M21 3a1 1 0 0 1 1 1v16a1 1 0 0 1-1 1H3a1 1 0 0 1-1-1V4a1 1 0 0 1 1-1zM7.416 14H4v5h16v-5h-3.416a5.001 5.001 0 0 1-9.168 0M20 5H4v7h5a3 3 0 1 0 6 0h5z"
      />
    </svg>;
  };
};

module LinkM = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="m17.657 14.828l-1.415-1.414L17.658 12A4 4 0 1 0 12 6.343l-1.414 1.414L9.17 6.343l1.415-1.414a6 6 0 0 1 8.485 8.485zm-2.829 2.829l-1.414 1.414a6 6 0 0 1-8.485-8.485l1.414-1.414l1.414 1.414L6.343 12A4 4 0 0 0 12 17.657l1.414-1.414zm0-9.9l1.415 1.415l-7.072 7.07l-1.414-1.414z"
      />
    </svg>;
  };
};

module LockPasswordLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M18 8h2a1 1 0 0 1 1 1v12a1 1 0 0 1-1 1H4a1 1 0 0 1-1-1V9a1 1 0 0 1 1-1h2V7a6 6 0 1 1 12 0zM5 10v10h14V10zm6 4h2v2h-2zm-4 0h2v2H7zm8 0h2v2h-2zm1-6V7a4 4 0 0 0-8 0v1z"
      />
    </svg>;
  };
};

module MailLockLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="m20 7.238l-7.928 7.1L4 7.216V19h9v2H3a1 1 0 0 1-1-1V4a1 1 0 0 1 1-1h18a1 1 0 0 1 1 1v7h-2zM19.501 5H4.511l7.55 6.662zM22 17h1v5h-8v-5h1v-1a3 3 0 1 1 6 0zm-2 0v-1a1 1 0 1 0-2 0v1z"
      />
    </svg>;
  };
};

module OCamlFileIcon = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 512 512">
      <path
        fill="currentColor"
        d="M0 223.6V97.4c0-35.6 28.9-64.5 64.5-64.5h383c35.6 0 64.5 28.9 64.5 64.5V479H299.5s-14.1-37-16.8-55.2c-.7-4.7.7-9.4-.8-13.8c-1.2-3.8-4-4.2-5.8-7.3c-4.8-8.2-9.8-18.8-10.2-28.8c-.4-9-3.7-17.2-4.2-26.1c-.2-4.3.3-8.8.1-13c-.1-2.1-.3-6.9-1.3-9.9l1.1-2.8c-.5-1 9.6-.6 12.6-.6c5.1.1 9.9.3 15 .6c10.4.5 19.8.4 29.9-1.2c22.5-3.5 32.9-12.7 38.2-16.5c20.7-14.9 30.1-39.3 30.1-39.3c3.4-7.6 3.4-21.2 10.7-27.3c8.6-7.2 23.1-6.7 33-11.1c5.8-2.6 10-4 15.9-2.7c4.4.9 12.3 6 14.1-1.1c-1.5-.9-2-2.7-2.8-3.6c8.2-.8.2-19.7-3.1-23.5c-5-5.8-13.3-8.5-22.1-10.8c-10.5-2.8-20-6-29.9-4.1c-17.3 3.4-16-6.5-26.2-6.5c-12.2 0-33.9.6-37.7 12.5c-1.7 5.5-3.6 5.8-6.6 10c-2.6 3.6.5 6.9-.9 11c-1.3 4.3-3.3 19.3-5.4 24.6c-3.5 8.9-7.6 20-15.3 20c-10.7 1.3-19.2 1.7-27.9-1.5c-5.2-1.9-14-4.9-18.4-6.7c-20.1-8.4-23.4-17.7-23.4-17.7c-2.2-3.6-23.4-47.8-26.5-55.4c-4-9.9-9.6-17.2-13.6-20.9c-6.2-5.6-12-14.2-24.7-11.7c-2.3.4-10.5.8-16.8 6.1c-4.3 3.6-5.6 11-9.6 17.3c-2.3 3.6-6.3 14-10 22.6c-2.6 6-3.8 10.5-6.5 12.7c-2.2 1.7-7.5 6-14.4-1c-2.6-2.7-9-13.9-13.7-22c-13.7-23.8-19.1-34.5-34.8-34.6c-20-.1-21.5 11.2-30.3 27.5c0-.1-8.8 33.1-40.5 56.4m0 146.1V479h99.2s8-30.9 9.8-37.2c1.5-4.8 3.4-8.7 4.2-13.6c.7-4.7-.1-9.2-.5-13.4c-1-10.7 7.8-14.5 12-23.7c3.8-8.3 6-17.7 9.1-26.2c3-8.1 7.7-19.6 15.8-23.7c-1-1.1-16.8-1.6-21-2.1c-13-1.4-31.5-5.3-39.9-7.3c-5.1-1.2-22.1-7.3-25.8-9c-8.7-4-14.4-14.8-21.2-13.7c-4.3.7-8.5 2.2-11.2 6.5c-2.2 3.6-2.9 9.7-4.5 13.8c-1.8 4.8-4.8 9.2-7.5 13.7c-4.7 8.3-15.4 17.3-18.5 26.6m275.9 92.6c-1.9-4-4.3-11.8-5.9-15.3c-1.5-3.2-6.1-11.6-8.4-14.3c-5-5.9-6.2-6.3-7.7-13.7c-2.6-12.9-9.3-36.3-17.3-52.4c-4.1-8.3-11-15.3-17.3-21.3c-5.5-5.3-17.8-14.2-20-13.8c-20.2 4-26.4 23.8-35.9 39.5c-5.3 8.7-10.8 16-15 25.3c-3.8 8.5-3.5 17.9-10 25.2c-6.7 7.5-11.1 15.4-14.4 25.1c-.6 1.8-6.3 32.4-6.3 32.4h164.7l-.3-.7c-2.2-6.3-5.1-13.6-6.2-16"
      />
    </svg>;
  };
};

module QuestionLink = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M12 22C6.477 22 2 17.523 2 12S6.477 2 12 2s10 4.477 10 10s-4.477 10-10 10m0-2a8 8 0 1 0 0-16a8 8 0 0 0 0 16m-1-5h2v2h-2zm2-1.645V14h-2v-1.5a1 1 0 0 1 1-1a1.5 1.5 0 1 0-1.471-1.794l-1.962-.393A3.501 3.501 0 1 1 13 13.355"
      />
    </svg>;
  };
};

module QuillLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M6.94 14.033a29.79 29.79 0 0 0-.606 1.783c.96-.697 2.101-1.14 3.418-1.304c2.513-.314 4.746-1.973 5.876-4.058l-1.456-1.455l1.413-1.415l1-1.002c.43-.429.915-1.224 1.428-2.367c-5.593.867-9.018 4.291-11.074 9.818M17 8.997l1 1c-1 3-4 6-8 6.5c-2.669.333-4.336 2.167-5.002 5.5H3c1-6 3-20 18-20c-1 2.997-1.998 4.996-2.997 5.997z"
      />
    </svg>;
  };
};

module RepositoryLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M13 21v2.5l-3-2l-3 2V21h-.5A3.5 3.5 0 0 1 3 17.5V5a3 3 0 0 1 3-3h14a1 1 0 0 1 1 1v17a1 1 0 0 1-1 1zm0-2h6v-3H6.5a1.5 1.5 0 0 0 0 3H7v-2h6zm6-5V4H6v10.035c.163-.023.33-.035.5-.035zM7 5h2v2H7zm0 3h2v2H7zm0 3h2v2H7z"
      />
    </svg>;
  };
};

module RingResize = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <g stroke="currentColor">
        <circle
          cx="12"
          cy="12"
          r="9.5"
          fill="none"
          strokeLinecap="round"
          strokeWidth="3">
          <animate
            attributeName="stroke-dasharray"
            calcMode="spline"
            dur="1.5s"
            keySplines="0.42,0,0.58,1;0.42,0,0.58,1;0.42,0,0.58,1"
            keyTimes="0;0.475;0.95;1"
            repeatCount="indefinite"
            values="0 150;42 150;42 150;42 150"
          />
          <animate
            attributeName="stroke-dashoffset"
            calcMode="spline"
            dur="1.5s"
            keySplines="0.42,0,0.58,1;0.42,0,0.58,1;0.42,0,0.58,1"
            keyTimes="0;0.475;0.95;1"
            repeatCount="indefinite"
            values="0;-16;-59;-59"
          />
        </circle>
        <animateTransform
          attributeName="transform"
          dur="2s"
          repeatCount="indefinite"
          type_="rotate"
          values="0 12 12;360 12 12"
        />
      </g>
    </svg>;
  };
};

module SparkleLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M14 4.438A2.437 2.437 0 0 0 16.438 2h1.125A2.437 2.437 0 0 0 20 4.438v1.125A2.437 2.437 0 0 0 17.563 8h-1.125A2.437 2.437 0 0 0 14 5.563zM1 11a6 6 0 0 0 6-6h2a6 6 0 0 0 6 6v2a6 6 0 0 0-6 6H7a6 6 0 0 0-6-6zm3.876 1A8.038 8.038 0 0 1 8 15.124A8.038 8.038 0 0 1 11.124 12A8.038 8.038 0 0 1 8 8.876A8.038 8.038 0 0 1 4.876 12m12.374 2A3.25 3.25 0 0 1 14 17.25v1.5A3.25 3.25 0 0 1 17.25 22h1.5A3.25 3.25 0 0 1 22 18.75v-1.5A3.25 3.25 0 0 1 18.75 14z"
      />
    </svg>;
  };
};

module UserLine = {
  [@react.component]
  let make = (~size="1rem", ~className="") => {
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M20 22h-2v-2a3 3 0 0 0-3-3H9a3 3 0 0 0-3 3v2H4v-2a5 5 0 0 1 5-5h6a5 5 0 0 1 5 5zm-8-9a6 6 0 1 1 0-12a6 6 0 0 1 0 12m0-2a4 4 0 1 0 0-8a4 4 0 0 0 0 8"
      />
    </svg>;
  };
};
