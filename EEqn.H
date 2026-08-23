{
    const surfaceScalarField phiv
    (
        fvc::absolute(phi/fvc::interpolate(rho), U)
    );

    fvScalarMatrix EEqn
    (
        fvm::ddt(rho, e) + fvm::div(phi, e)
      + fvc::div(phi*Af, e)/A - fvc::div(phi, e)
      + fvc::ddt(rho, K) + fvc::div(phi*Af, K)/A
      + fvc::div(phiv*Af, p, "div(phiv,p)")/A
      - fvm::laplacian(turbulence->alphaEff(), e)
     ==
        fvOptions(rho, e)
      + (ef+0.5*pow(vf,2))*(fvc::grad(mdot) & vector(1, 0, 0))/A
      + (fvc::grad(Qdot) & vector(1, 0, 0))/A
    );

    if (MRF.active())
    {
        EEqn += fvc::div(MRF.phi(), p);
    }

    EEqn.relax();

    fvOptions.constrain(EEqn);

    EEqn.solve();

    fvOptions.correct(e);

    thermo.correct();
}
